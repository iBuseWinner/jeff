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

/*! @brief Matches the answer based on the input. */
void JCK::search_for_suggests(const QString &input) {
  Yellog::Trace("Searching in JCK now: %s", input.toStdString().c_str());
  if (scanner) {
    Yellog::Trace("\tRequesting in custom scanner...");
    auto json_object = pw->request_scan(scanner, input);
    if (json_object.contains(basis->sendWk)) emit response(json_object[basis->sendWk].toString());
    return;
  }
  CacheWithIndices selection;
  bool from_db = false;
  /*! If user sent the same message as previous one, we should update cache from database and unset use cases counter. */
  if (hp->last_user_message(1) == input) {
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
  if (selection.isEmpty()) {
    Yellog::Trace("\tNo reagent found, emitted empty.");
    emit empty(input);
    return;
  }
  auto sorted = select_candidates(selection);
  if (composer) {
    auto json_object = pw->request_compose(composer, input, sorted);
    if (json_object.contains(basis->sendWk)) {
      emit response(json_object[basis->sendWk].toString());
      return;
    }
  }
  auto composition = compose_answer(input, sorted);
  if (composition.first.length() / input.length() > 0.33 and not from_db) {
    selection = select_from_db(input);
    if (selection.isEmpty()) {
      emit empty(input);
      return;
    }
    sorted = select_candidates(selection);
    composition = compose_answer(input, sorted);
  }
  emit response(composition.second.trimmed());
}

/*! @brief Resets the expression usage counter. */
void JCK::reset_cache_use_cases(CacheWithIndices &selection) {
  auto *cache = basis->cacher->get_ptr();
  for (auto expr : selection)
    for (int i = 0; i < cache->length(); i++)
      if (expr.second.activator_text == (*cache)[i].activator_text) (*cache)[i].use_cases = 0;
}

/*! @brief Matches candidates for each index of occurrence in the input expression.
 *  @details If occurrences in the user's phrase do not intersect (@sa StringSearch::intersects),
 *  the phrases are added to the list in order of occurrences of their activators in the input.
 *  If occurrences intersect, the most significant, the most rarely used, or random otherwise is selected.  */
CacheWithIndices JCK::select_candidates(CacheWithIndices selection) {
  Yellog::Trace("\tLet's select candidates:");
  CacheWithIndices candidates;
  for (auto ewi : selection) {
    Yellog::Trace("\t\tCandidate: activator is \"%s\", reagent is \"%s\"",
                  ewi.second.activator_text.toStdString().c_str(),
                  ewi.second.reagent_text.toStdString().c_str());
    Yellog::Trace("\t\tIndices are:");
    for (auto key : ewi.first.keys()) {
      Yellog::Trace("\t\t\t%d-%d", key, ewi.first[key]);
    }
    if (candidates.isEmpty()) {
      Yellog::Trace("\t\tFirst candidate found");
      candidates.append(ewi);
      continue;
    }
    bool to_add = false;
    if (ewi.second.consonant()) {
      Yellog::Trace("\t\tIt's consonant expression");
      to_add = true;
    } else {
      Yellog::Trace("\t\tIterating with its rivals:");
      QMutableListIterator<ExpressionWithIndices> rivals_iter(candidates);
      while (rivals_iter.hasNext()) {
        auto &rival = rivals_iter.next();
        auto intersection_and_weight = StringSearch::intersects(ewi.first, rival.first);
        auto x = intersection_and_weight.first;
        auto weight_sub = intersection_and_weight.second;
        if (x == StringSearch::Intersects::No) {
          Yellog::Trace("\t\t\tNo intersection \"%s\"-\"%s\" with \"%s\"-\"%s\"",
                        ewi.second.activator_text.toStdString().c_str(),
                        ewi.second.reagent_text.toStdString().c_str(),
                        rival.second.activator_text.toStdString().c_str(),
                        rival.second.reagent_text.toStdString().c_str());
          to_add = true;
        }
        else {
          weight_sub += ewi.second.weight() - rival.second.weight();
          if (weight_sub > 0) {
            Yellog::Trace("\t\t\tWeight of \"%s\"-\"%s\" bigger than \"%s\"-\"%s\"",
                        ewi.second.activator_text.toStdString().c_str(),
                        ewi.second.reagent_text.toStdString().c_str(),
                        rival.second.activator_text.toStdString().c_str(),
                        rival.second.reagent_text.toStdString().c_str());
            to_add = true;
            rivals_iter.remove();
            continue;
          }
          if (weight_sub == 0) {
            if (ewi.second.use_cases < rival.second.use_cases) {
              Yellog::Trace("\t\t\tExpression \"%s\"-\"%s\" is more rarely used than \"%s\"-\"%s\"",
                        ewi.second.activator_text.toStdString().c_str(),
                        ewi.second.reagent_text.toStdString().c_str(),
                        rival.second.activator_text.toStdString().c_str(),
                        rival.second.reagent_text.toStdString().c_str());
              to_add = true;
              rivals_iter.remove();
              continue;
            }
            if (ewi.second.use_cases == rival.second.use_cases) {
              if (gen->bounded(0, 2) == 1) {
                Yellog::Trace("\t\t\tRandomly selected \"%s\"-\"%s\" over \"%s\"-\"%s\"",
                        ewi.second.activator_text.toStdString().c_str(),
                        ewi.second.reagent_text.toStdString().c_str(),
                        rival.second.activator_text.toStdString().c_str(),
                        rival.second.reagent_text.toStdString().c_str());
                to_add = true;
                rivals_iter.remove();
              }
            }
          }
        }
      }
    }
    if (to_add) {
      bool inserted = false;
      QMutableListIterator<ExpressionWithIndices> candy_iter(candidates);
      Yellog::Trace("\t\tInserting:");
      while (candy_iter.hasNext()) {
        auto &candidate = candy_iter.next();
        Yellog::Trace("\t\t\tIndex of current listed candidate is %d, of our ewi is %d",
                      candidate.first.keys()[0], ewi.first.keys()[0]);
        if (candidate.first.keys()[0] > ewi.first.keys()[0]) {
          Yellog::Trace("\t\t\tInserted");
          candy_iter.previous();
          candy_iter.insert(ewi);
          inserted = true;
          break;
        }
      }
      if (not inserted) {
        Yellog::Trace("\t\t\tInserted at the end");
        candidates.append(ewi);
      }
    }
  }
  return candidates;
}

/*! @brief Builds an answer in the order of the indices of the occurrence and
 *  fills the uncovered part of the expression.  */
QPair<QString, QString> JCK::compose_answer(QString input, CacheWithIndices candidates) {
  QString output;
  auto lemmatized = StringSearch::lemmatize(input);
  for (auto ewi : candidates) {
    if (ewi.second.exec) {
      /*! Executable <=> React Script. @sa ScriptMeta or ExtensionMeta */
      auto *script_meta = ScriptMeta::from_string(ewi.second.reagent_text);
      if (script_meta) {
        if (script_meta->stype != ScriptType::React) {
          delete script_meta;
          continue;
        }
        /*! React scripts. @sa ScriptType::React */
        QJsonObject obj;
        if (script_meta->is_for_embedded_python) {
          obj = pw->request_answer(script_meta, ewi.second, input);
        } else {
          obj = DaemonProcess::request_output(script_meta, ewi.second, input);
        }
        delete script_meta;
        if (obj.contains(basis->errorTypeWk)) continue;
        if (obj.contains(basis->sendWk)) {
          ewi.second.use_cases += 1;
          basis->cacher->append(ewi.second);
          output += obj[basis->sendWk].toString() + " ";
        }
      } else {
        auto *extension_meta = ExtensionMeta::from_string(ewi.second.reagent_text);
        if (extension_meta) {
          /*! Some extension. @sa ExtensionMeta */
          emit start_extension(extension_meta);
        }
      }
    } else {
      /*! Not `exec` */
      ewi.second.use_cases += 1;
      basis->cacher->append(ewi.second);
      output += ewi.second.reagent_text + " ";
    }
    if (not ewi.second.consonant()) lemmatized = StringSearch::replace(lemmatized, ewi.first);
  }
  return QPair<QString, QString>(lemmatized, output);
}

/*! @brief Selects expressions to compose a response from the cache. */
CacheWithIndices JCK::select_from_cache(const QString &input) {
  Yellog::Trace("\tSelecting from cache: %s", input.toStdString().c_str());
  CacheWithIndices selection;
  Cache cache = basis->cacher->get();
  for (int i = 0; i < cache.length(); i++) {
    auto x = StringSearch::contains(input, cache[i].activator_text);
    if (x.contains(0)) if (x[0] == 0) continue;
    for (auto ewi : selection) if (ewi.second == cache[i]) continue;
    Yellog::Trace("\t\tChoosed \"%s\": reagent is \"%s\"",
                  cache[i].activator_text.toStdString().c_str(),
                  cache[i].reagent_text.toStdString().c_str());
    for (auto key : x.keys()) { Yellog::Trace("\t\tIts indices are %d-%d.", key, x[key]); }
    selection.append(ExpressionWithIndices(x, cache[i]));
  }
  return selection;
}

/*! @brief Selects expressions to compose a response from the database. */
CacheWithIndices JCK::select_from_db(const QString &input) {
  Yellog::Trace("\tSelecting from database: %s", input.toStdString().c_str());
  CacheWithIndices selection;
  Sources sources = basis->sources();
  for (int i = 0; i < sources.length(); i++) {
    auto cwi = basis->sql->scan_source(sources[i], input, thread_conn_wk);
    for (auto ewi : cwi) {
      Yellog::Trace("\t\tChoosed \"%s\": reagent is \"%s\"",
                    ewi.second.activator_text.toStdString().c_str(),
                    ewi.second.reagent_text.toStdString().c_str());
      for (auto key : ewi.first.keys()) { Yellog::Trace("\t\tIts indices are %d-%d.", key, ewi.first[key]); }
      // Appending source weight:
      ewi.second.properties["weight"] = ewi.second.weight() + sources[i].weight;
      // Removing duplicates:
      for (auto _ewi : selection) if (_ewi.second == ewi.second) continue;
      // Inserting in cache:
      selection.append(ewi);
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
