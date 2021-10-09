#include "nlp-module.h"

/*!
 * @fn NLPmodule::select_candidates
 * @brief Matches candidates for each index of occurrence in the input
 * expression.
 * @param[in] selection selected expressions
 * @param[in] input text string
 * @returns dictionary with indices and selected expressions
 */
CacheWithIndices NLPmodule::select_candidates(CacheWithIndices selection,
                                              QString input) {
  std::cout << "--------" << std::endl;
  std::cout << "\tfn select_candidates" << std::endl;
  std::cout << "\tGot input as \"" << input.toStdString() << "\"" << std::endl;
  CacheWithIndices candidates;
  for (auto ewi : selection) {
    if (candidates.keys().isEmpty()) {
      std::cout << "\tFirst ewi is \"" << ewi.second->activator_text.toStdString() << "\"" << std::endl;
      candidates[0] = ewi;
      continue;
    }
    bool not_in_candidates = true;
    for (auto rival : candidates.keys()) {
      std::cout << "\t *Got rival = " << rival << std::endl;
      auto x = StringSearch::intersects(ewi.first, candidates[rival].first);
      if (x == Intersects::FirstBetter) {
        std::cout << "\t  Got first better" << std::endl;
        not_in_candidates = false;
        candidates.remove(rival);
      } else if (x == Intersects::Equal) {
        std::cout << "\t  Got equals" << std::endl;
        if (ewi.second->use_cases > candidates[rival].second->use_cases) {
          not_in_candidates = false;
          candidates.remove(rival);
        } else if (ewi.second->use_cases ==
                   candidates[rival].second->use_cases) {
          if (_gen->bounded(0, 2) == 1) {
            not_in_candidates = false;
            candidates.remove(rival);
          }
        }
      }
    }
    if (not_in_candidates) {
      std::cout << "\tAppending at " << candidates.lastKey() + 1 << " ewi as \"" << ewi.second->activator_text.toStdString() << "\"" << std::endl;
      candidates[candidates.lastKey() + 1] = ewi;
    }
  }
  return candidates;
}

/*!
 * @fn NLPmodule::compose_answer
 * @brief Builds an answer in the order of the indices of the occurrence and
 * sets the uncovered part of the expression.
 * @param[in,out] input (out as QPair::first) text input
 * @param[in] candidates to be printed
 * @returns QPair of @a input and @a output
 */
QPair<QString, QString> NLPmodule::compose_answer(QString input,
                                                  CacheWithIndices candidates) {
  QString output;
  std::cout << "--------" << std::endl;
  std::cout << "\tfn compose_answer" << std::endl;
  std::cout << "\tGot input as \"" << input.toStdString() << "\"" << std::endl;
  input = StringSearch::purify(input);
  for (auto ewi : candidates) {
    if (not ewi.second) {
      std::cout << "\tWWW" << std::endl;
      continue;
    }
    std::cout << "\tHere we go again. Let's try:" << std::endl;
    ewi.second->use_cases += 1;
    if (not _cache.contains(ewi.second)) {
      std::cout << "\tNew expression in cache!" << std::endl;
      _cache.append(ewi.second);
    }
    output += ewi.second->reagent_text + " ";
    input = StringSearch::replace(input, ewi.first);
  }
  return QPair<QString, QString>(input, output);
}

/*!
 * @fn NLPmodule::search_for_suggests
 * @brief Matches the answer based on the input.
 * @param[in] input user input
 */
void NLPmodule::search_for_suggests(const QString &input) {
  auto selection = select_from_cache(input);
  bool from_db = false;
  if (selection.keys().isEmpty()) {
    selection = select_from_db(input);
    from_db = true;
  }
  std::cout << "--------" << std::endl;
  std::cout << "\tfn search_for_suggests" << std::endl;
  std::cout << "\tGot input as \"" << input.toStdString() << "\"" << std::endl;
  std::cout << "\tCalculated selection.keys().length() = "
            << selection.keys().length() << std::endl;
  std::cout << "\tSelection is from DB? " << from_db << std::endl;
  if (selection.keys().isEmpty())
    return;
  auto sorted = select_candidates(selection, input);
  auto composition = compose_answer(input, sorted);
  if (composition.first.length() / input.length() > 0.33 and not from_db) {
    selection = select_from_db(input);
    if (selection.keys().isEmpty())
      return;
    sorted = select_candidates(selection, input);
    composition = compose_answer(input, sorted);
  }
  std::cout << "--------" << std::endl;
  std::cout << "\tfn search_for_suggests (requested)" << std::endl;
  std::cout << "\tCalculated response: \"" << composition.second.toStdString()
            << "\"" << std::endl;
  emit response(composition.second.trimmed());
}

/*!
 * @fn NLPmodule::select_from_cache
 * @brief Selects expressions to compose a response from the cache.
 * @param[in] input user input
 * @returns matching expressions
 */
CacheWithIndices NLPmodule::select_from_cache(const QString &input) {
  CacheWithIndices selection;
  std::cout << "--------" << std::endl;
  std::cout << "\tfn select_from_cache" << std::endl;
  std::cout << "\tCache:" << std::endl;
  for (auto *ex : _cache)
    std::cout << "\t  \"" << ex->activator_text.toStdString() << "\""
              << std::endl;
  for (int i = 0; i < _cache.length(); i++) {
    auto x = StringSearch::contains(input, _cache[i]->activator_text);
    if (x[0] != 0) {
      bool is_in = false;
      for (auto ewi : selection)
        if (ewi.second == _cache[i])
          is_in = true;
      if (is_in)
        continue;
      if (selection.keys().length() == 0)
        selection[0] = ExpressionWithIndices(x, _cache[i]);
      else
        selection[selection.lastKey() + 1] =
            ExpressionWithIndices(x, _cache[i]);
    }
  }
  return selection;
}

/*!
 * @fn NLPmodule::select_from_db
 * @brief Selects expressions to compose a response from the database.
 * @param[in] input user input
 * @returns matching expressions
 */
CacheWithIndices NLPmodule::select_from_db(const QString &input) {
  CacheWithIndices selection;
  Sources sources = _basis->get_sources();
  for (int i = 0; i < sources.length(); i++) {
    auto cwi = _basis->sql->scan_source(sources[i], input);
    for (auto ewi : cwi) {
      bool is_in = false;
      for (auto _ewi : selection)
        if (_ewi.second == ewi.second)
          is_in = true;
      if (is_in)
        continue;
      if (selection.keys().length() == 0)
        selection[0] = ewi;
      else 
        selection[selection.lastKey() + 1] = ewi;
    }
  }
  return selection;
}

/*!
 * @fn NLPmodule::load_cache
 * @brief Loads the cache into memory.
 */
void NLPmodule::load_cache() { _cache = _basis->json->read_NLP_cache(); }

/*!
 * @fn NLPmodule::save_cache
 * @brief Saves the cache to disk.
 */
void NLPmodule::save_cache() { _basis->json->write_NLP_cache(_cache); }
