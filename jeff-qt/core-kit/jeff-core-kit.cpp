#include "jeff-core-kit.h"

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
  if (scanner) {
    auto json_object = pw->request_scan(scanner, input);
    if (json_object.contains(basis->sendWk)) emit response(json_object[basis->sendWk].toString());
    return;
  }
  CacheWithIndices selection;
  bool from_db = false;
  /*! If user sent the same message as previous one, we should update cache from database
   *  and unset use cases counter.  */
  if (hp->last_user_message(1) == input) {
    selection = select_from_db(input);
    reset_cache_use_cases(selection);
    from_db = true;
  } else {
    selection = select_from_cache(input);
    if (selection.keys().isEmpty()) {
      selection = select_from_db(input);
      from_db = true;
    }
  }
  if (selection.keys().isEmpty()) {
    emit empty(input);
    return;
  }
  auto sorted = select_candidates(selection, input);
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
    if (selection.keys().isEmpty()) {
      emit empty(input);
      return;
    }
    sorted = select_candidates(selection, input);
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

/*! @brief Matches candidates for each index of occurrence in the input expression. */
CacheWithIndices JCK::select_candidates(CacheWithIndices selection, QString input) {
  CacheWithIndices candidates;
  for (auto ewi : selection) {
    if (candidates.keys().isEmpty()) {
      candidates[0] = ewi;
      continue;
    }
    bool to_add = false;
    if (ewi.second.consonant()) { to_add = true; }
    else {
      for (auto rival : candidates.keys()) {
        auto intersection_and_weight = StringSearch::intersects(ewi.first, candidates[rival].first);
        auto x = intersection_and_weight.first;
        auto weight_sub = intersection_and_weight.second;
        if (x == StringSearch::Intersects::No) to_add = true;
        else {
          weight_sub += ewi.second.weight() - candidates[rival].second.weight();
          if (weight_sub > 0) {
            to_add = true;
            candidates.remove(rival);
            continue;
          }
          if (weight_sub == 0) {
            if (ewi.second.use_cases < candidates[rival].second.use_cases) {
              to_add = true;
              candidates.remove(rival);
              continue;
            }
            if (ewi.second.use_cases == candidates[rival].second.use_cases) {
              if (gen->bounded(0, 2) == 1) {
                to_add = true;
                candidates.remove(rival);
              }
            }
          }
        }
      }
    }
    if (to_add) {
      if (not candidates.isEmpty()) candidates[candidates.lastKey() + 1] = ewi;
      else candidates[0] = ewi;
    }
  }
  return candidates;
}

/*! @brief Builds an answer in the order of the indices of the occurrence and
 *  fills the uncovered part of the expression.  */
QPair<QString, QString> JCK::compose_answer(QString input, CacheWithIndices candidates) {
  QString output;
  input = StringSearch::lemmatize(input);
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
    if (not ewi.second.consonant()) input = StringSearch::replace(input, ewi.first);
  }
  return QPair<QString, QString>(input, output);
}

/*! @brief Selects expressions to compose a response from the cache. */
CacheWithIndices JCK::select_from_cache(const QString &input) {
  CacheWithIndices selection;
  Cache cache = basis->cacher->get();
  for (int i = 0; i < cache.length(); i++) {
    auto x = StringSearch::contains(input, cache[i].activator_text);
    if (x[0] != 0) {
      for (auto ewi : selection) if (ewi.second == cache[i]) continue;
      if (selection.keys().length() == 0) selection[0] = ExpressionWithIndices(x, cache[i]);
      else selection[selection.lastKey() + 1] = ExpressionWithIndices(x, cache[i]);
    }
  }
  return selection;
}

/*! @brief Selects expressions to compose a response from the database. */
CacheWithIndices JCK::select_from_db(const QString &input) {
  CacheWithIndices selection;
  Sources sources = basis->sources();
  for (int i = 0; i < sources.length(); i++) {
    auto cwi = basis->sql->scan_source(sources[i], input, thread_conn_wk);
    for (auto ewi : cwi) {
      for (auto _ewi : selection) if (_ewi.second == ewi.second) continue; 
      if (selection.keys().length() == 0) selection[0] = ewi;
      else selection[selection.lastKey() + 1] = ewi;
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
void JCK::set_custom_composer(ScriptMeta *custom_composer) {
  composer = custom_composer;
}
