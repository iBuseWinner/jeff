#include "jeff-core-kit.hpp"

/*! @brief The constructor. */
JCK::JCK(Basis *_basis, HProcessor *_hp, QObject *parent)
    : QObject(parent), basis(_basis), hp(_hp) {
  gen = new QRandomGenerator(QTime::currentTime().msec());
  load_cache();
}

/*! @brief The destructor. */
JCK::~JCK() { save_cache(); delete gen; }

/*! @brief The thread setup. */
void JCK::thread_setup() {
  pw = new PythonWorker(basis, hp, this);
  connect(pw, &PythonWorker::script_exception, this, [this](QString e) {
    emit script_exception(e);
  });
  QSqlDatabase::addDatabase("QSQLITE", thread_conn_wk);
}

/*! @brief TBD */
CoverageCache JCK::get_from_json(const QJsonArray &array) {
  CoverageCache ccache;
  for (auto ec_val : array) {
    if (ec_val.isObject()) {
      auto ec_obj = ec_val.toObject();
      ExpressionCoverage ec;
      if (ec_obj.contains("expression")) {
        bool err = false;
        auto expression = Expression::from_object(ec_obj["expression"].toObject(), &err);
        if (not err) ec.expression = expression;
        else continue;
      }
      if (ec_obj.contains("total_POC")) ec.total_POC = float(ec_obj["total_POC"].toDouble(0.0));
      if (ec_obj.contains("coverage_indices")) {
        auto ecci_val = ec_obj["coverage_indices"];
        if (ecci_val.isArray()) {
          auto ecci_arr = ecci_val.toArray();
          QMap<int, int> coverage_indices;
          for (auto ecci_wi_val : ecci_arr) {
            if (ecci_wi_val.isArray()) {
              auto ecci_wi_arr = ecci_wi_val.toArray();
              if (ecci_wi_arr.size() != 2) continue;
              int i1 = ecci_wi_arr[0].toInt(-1);
              if (i1 < 0) continue;
              int i2 = ecci_wi_arr[1].toInt(-1);
              if (i2 <= 0) continue;
              coverage_indices[i1] = i2;
            }
          }
          if (coverage_indices.keys().isEmpty()) continue;
          ec.coverage_indices = coverage_indices;
        }
      }
      ccache.append(ec);
    }
  }
  return ccache;
}

/*! @brief Matches the answer based on the input. */
void JCK::search_for_suggests(const QString &input) {
  Yellog::Trace("Searching in JCK now: %s", input.toStdString().c_str());
  CoverageCache selection;
  bool from_db = false;
  if (scanner) {
    Yellog::Trace("\tRequesting in custom scanner...");
    from_db = true;
    auto json_object = pw->request_scan(scanner, basis->sources(), input);
    if (json_object.contains(basis->sendWk)) {
      /*! @details If the scanner immediately gives the final answer, we send it. */
      emit response(json_object[basis->sendWk].toString());
      return;
    } else if (json_object.contains(basis->selectionWk)) {
      auto selection_arr = json_object[basis->selectionWk].toArray();
      selection = get_from_json(selection_arr);
    } else return;
  } else {
    /*! @details If user sent the same message as previous one, we should update cache from database and unset use cases counter.
     *  @note Only when setting is setted.  */
    if ((hp->last_user_message(1) == input) and (*basis)[basis->seacrhInDbWhenRepeated].toBool()) {
      Yellog::Trace("\tFound same as the last");
      selection = select_from_db(input);
      reset_cache_use_cases(selection);
      from_db = true;
    } else {
      selection = select_from_cache(input);
      if (selection.isEmpty()) {
        selection = select_from_db(input);
        from_db = true;
      }
    }
  }
  if (selection.isEmpty()) {
    Yellog::Trace("\tNo reagent found, emitted empty.");
    emit empty(input);
    return;
  }
  auto sorted = select_candidates(selection);
  QPair<QString, QString> composition;
  if (composer) {
    auto json_object = pw->request_compose(composer, input, sorted);
    if (json_object.contains(basis->sendWk)) emit response(json_object[basis->sendWk].toString());
    else emit empty(input);
  } else {
    composition = compose_answer(input, sorted);
    if (composition.first.length() / input.length() > 0.33 and not from_db) {
      selection = select_from_db(input);
      if (selection.isEmpty()) { emit empty(input); return; }
      sorted = select_candidates(selection);
      composition = compose_answer(input, sorted);
    }
    if (composition.second.length()) emit response(composition.second.trimmed());
    else emit empty(input);
  }
}

/*! @brief Resets the expression usage counter. */
void JCK::reset_cache_use_cases(CoverageCache &selection) {
  auto *cache = basis->cacher->get_ptr();
  for (auto ec : selection)
    for (int i = 0; i < cache->length(); i++)
      if (ec.expression.activator_text == (*cache)[i].activator_text) (*cache)[i].use_cases = 0;
}

/*! @brief Matches candidates for each index of occurrence in the input expression.
 *  @details If occurrences in the user's phrase do not intersect (@sa StringSearch::intersects),
 *  the phrases are added to the list in order of occurrences of their activators in the input.
 *  If occurrences intersect, the most significant, the most rarely used, or random otherwise is selected.  */
CoverageCache JCK::select_candidates(CoverageCache selection) {
  Yellog::Trace("\tLet's select candidates:");
  CoverageCache candidates;
  for (auto ec : selection) {
    Yellog::Trace("\t\tCandidate: activator is \"%s\", reagent is \"%s\"",
                  ec.expression.activator_text.toStdString().c_str(),
                  ec.expression.reagent_text.toStdString().c_str());
    Yellog::Trace("\t\tIndices are:");
    for (auto key : ec.coverage_indices.keys()) {
      Yellog::Trace("\t\t\t%d-%d", key, ec.coverage_indices[key]);
    }
    if (candidates.isEmpty()) {
      Yellog::Trace("\t\tFirst candidate found");
      candidates.append(ec);
      continue;
    }
    bool to_add = false;
    if (ec.expression.consonant()) {
      Yellog::Trace("\t\tIt's consonant expression");
      to_add = true;
    } else {
      Yellog::Trace("\t\tIterating with its rivals:");
      bool intersects = false;
      QMutableListIterator<ExpressionCoverage> rivals_iter(candidates);
      while (rivals_iter.hasNext()) {
        auto &rival = rivals_iter.next();
        if (rival.expression.consonant()) continue;
        if (StringSearch::intersects(ec.coverage_indices, rival.coverage_indices) == StringSearch::Intersects::No) {
          Yellog::Trace("\t\t\tNo intersection \"%s\"-\"%s\" with \"%s\"-\"%s\"",
                        ec.expression.activator_text.toStdString().c_str(),
                        ec.expression.reagent_text.toStdString().c_str(),
                        rival.expression.activator_text.toStdString().c_str(),
                        rival.expression.reagent_text.toStdString().c_str());
        } else {
          intersects = true;
          if (ec.total_POC > rival.total_POC) {
            Yellog::Trace("\t\t\tThe \"%s\"-\"%s\" is more accurately activated than \"%s\"-\"%s\"",
                          ec.expression.activator_text.toStdString().c_str(),
                          ec.expression.reagent_text.toStdString().c_str(),
                          rival.expression.activator_text.toStdString().c_str(),
                          rival.expression.reagent_text.toStdString().c_str());
            to_add = true;
            rivals_iter.remove();
            continue;
          } else if (ec.total_POC == rival.total_POC) {
            auto weights = ec.expression.weight() - rival.expression.weight();
            if (weights > 0) {
              Yellog::Trace("\t\t\tWeight of \"%s\"-\"%s\" bigger than \"%s\"-\"%s\"",
                          ec.expression.activator_text.toStdString().c_str(),
                          ec.expression.reagent_text.toStdString().c_str(),
                          rival.expression.activator_text.toStdString().c_str(),
                          rival.expression.reagent_text.toStdString().c_str());
              to_add = true;
              rivals_iter.remove();
            } else if (weights == 0) {
              if (ec.expression.use_cases < rival.expression.use_cases) {
                Yellog::Trace("\t\t\tExpression \"%s\"-\"%s\" is more rarely used than \"%s\"-\"%s\"",
                          ec.expression.activator_text.toStdString().c_str(),
                          ec.expression.reagent_text.toStdString().c_str(),
                          rival.expression.activator_text.toStdString().c_str(),
                          rival.expression.reagent_text.toStdString().c_str());
                to_add = true;
                rivals_iter.remove();
              } else if (ec.expression.use_cases == rival.expression.use_cases) {
                if (gen->bounded(0, 2) == 1) {
                  Yellog::Trace("\t\t\tRandomly selected \"%s\"-\"%s\" over \"%s\"-\"%s\"",
                          ec.expression.activator_text.toStdString().c_str(),
                          ec.expression.reagent_text.toStdString().c_str(),
                          rival.expression.activator_text.toStdString().c_str(),
                          rival.expression.reagent_text.toStdString().c_str());
                  to_add = true;
                  rivals_iter.remove();
                }
              }
            }
          }
        }
      }
      if (not intersects) {
        Yellog::Trace("\t\tNo intersection with other expressions, including!");
        to_add = true;
      }
    }
    if (to_add) {
      bool inserted = false;
      QMutableListIterator<ExpressionCoverage> candy_iter(candidates);
      Yellog::Trace("\t\tInserting:");
      while (candy_iter.hasNext()) {
        auto &candidate = candy_iter.next();
        Yellog::Trace("\t\t\tIndex of current listed candidate is %d, of our ec is %d",
                      candidate.coverage_indices.keys()[0], ec.coverage_indices.keys()[0]);
        if (candidate.coverage_indices.keys()[0] > ec.coverage_indices.keys()[0]) {
          Yellog::Trace("\t\t\tInserted");
          candy_iter.previous();
          candy_iter.insert(ec);
          inserted = true;
          break;
        }
      }
      if (not inserted) {
        Yellog::Trace("\t\t\tInserted at the end");
        candidates.append(ec);
      }
    }
  }
  return candidates;
}

/*! @brief Builds an answer in the order of the indices of the occurrence and
 *  fills the uncovered part of the expression.  */
QPair<QString, QString> JCK::compose_answer(QString input, CoverageCache candidates) {
  QString output;
  auto lemmatized = StringSearch::lemmatize(input);
  for (auto ec : candidates) {
    if (ec.expression.exec) {
      /*! Executable <=> React Script. @sa ScriptMeta or ExtensionMeta */
      auto *script_meta = ScriptMeta::from_string(ec.expression.reagent_text);
      if (script_meta) {
        if (script_meta->stype != ScriptType::React) {
          delete script_meta;
          continue;
        }
        /*! React scripts. @sa ScriptType::React */
        QJsonObject obj;
        if (script_meta->is_for_embedded_python) {
          obj = pw->request_answer(script_meta, ec.expression, input);
        } else {
          obj = DaemonProcess::request_output(script_meta, ec.expression, input);
        }
        delete script_meta;
        if (obj.contains(basis->errorTypeWk)) continue;
        if (obj.contains(basis->sendWk)) {
          ec.expression.use_cases += 1;
          basis->cacher->append(ec.expression);
          output += obj[basis->sendWk].toString() + " ";
        }
      } else {
        auto *extension_meta = ExtensionMeta::from_string(ec.expression.reagent_text);
        if (extension_meta) {
          /*! Some extension. @sa ExtensionMeta */
          emit start_extension(extension_meta);
        }
      }
    } else {
      /*! Not `exec` */
      ec.expression.use_cases += 1;
      basis->cacher->append(ec.expression);
      output += ec.expression.reagent_text + " ";
    }
    if (not ec.expression.consonant()) lemmatized = StringSearch::replace(lemmatized, ec.coverage_indices);
  }
  return QPair<QString, QString>(lemmatized, output);
}

/*! @brief Selects expressions to compose a response from the cache. */
CoverageCache JCK::select_from_cache(const QString &input) {
  Yellog::Trace("\tSelecting from cache: %s", input.toStdString().c_str());
  CoverageCache selection;
  Cache cache = basis->cacher->get();
  for (int i = 0; i < cache.length(); i++) {
    auto indices_and_POC = StringSearch::contains(input, cache[i].activator_text);
    if (indices_and_POC.second == 0.0) continue;
    for (auto ec : selection) if (ec.expression == cache[i]) continue;
    Yellog::Trace("\t\tChoosed \"%s\": reagent is \"%s\"",
                  cache[i].activator_text.toStdString().c_str(),
                  cache[i].reagent_text.toStdString().c_str());
    for (auto key : indices_and_POC.first.keys()) {
      Yellog::Trace("\t\tIts indices are %d-%d.", key, indices_and_POC.first[key]);
    }
    ExpressionCoverage ec;
    ec.expression = cache[i];
    ec.coverage_indices = indices_and_POC.first;
    ec.total_POC = indices_and_POC.second;
    selection.append(ec);
  }
  return selection;
}

/*! @brief Selects expressions to compose a response from the database. */
CoverageCache JCK::select_from_db(const QString &input) {
  Yellog::Trace("\tSelecting from database: %s", input.toStdString().c_str());
  CoverageCache selection;
  Sources sources = basis->sources();
  for (int i = 0; i < sources.length(); i++) {
    auto cc = basis->sql->scan_source(sources[i], input, thread_conn_wk);
    for (auto ec : cc) {
      Yellog::Trace("\t\tChoosed \"%s\": reagent is \"%s\"",
                    ec.expression.activator_text.toStdString().c_str(),
                    ec.expression.reagent_text.toStdString().c_str());
      for (auto key : ec.coverage_indices.keys()) { Yellog::Trace("\t\tIts indices are %d-%d.", key, ec.coverage_indices[key]); }
      // Appending source weight:
      ec.expression.properties["weight"] = ec.expression.weight() + sources[i].weight;
      // Removing duplicates:
      for (auto _ec : selection) if (_ec.expression == ec.expression) continue;
      // Inserting in cache:
      basis->cacher->append(ec.expression);
      selection.append(ec);
    }
  }
  return selection;
}

/*! @brief Loads the cache into memory. */
void JCK::load_cache() { basis->cacher->append(basis->json->read_NLP_cache()); }
/*! @brief Saves the cache to disk. */
void JCK::save_cache() { basis->json->write_NLP_cache(basis->cacher->get()); }
/*! @brief Sets up custom scanner. */
void JCK::set_custom_scanner(ScriptMeta *custom_scanner) { scanner = custom_scanner; }
/*! @brief Sets up custom composer. */
void JCK::set_custom_composer(ScriptMeta *custom_composer) { composer = custom_composer; }
