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
  CacheWithIndices candidates;
  for (auto ewi : selection) {
    if (candidates.keys().isEmpty()) {
      candidates[0] = ewi;
      continue;
    }
    bool not_in_candidates = false;
    for (auto rival : candidates.keys()) {
      auto intersection_and_weight =
          StringSearch::intersects(ewi.first, candidates[rival].first);
      auto x = intersection_and_weight.first;
      auto weight_sub = intersection_and_weight.second;
      if (x == Intersects::No)
        not_in_candidates = true;
      else {
        weight_sub += ewi.second->weight() - candidates[rival].second->weight();
        if (weight_sub > 0) {
          not_in_candidates = true;
          candidates.remove(rival);
        } else if (weight_sub == 0) {
          if (ewi.second->use_cases < candidates[rival].second->use_cases) {
            not_in_candidates = true;
            candidates.remove(rival);
          } else if (ewi.second->use_cases ==
                     candidates[rival].second->use_cases) {
            if (_gen->bounded(0, 2) == 1) {
              not_in_candidates = true;
              candidates.remove(rival);
            }
          }
        }
      }
    }
    if (not_in_candidates)
      candidates[candidates.lastKey() + 1] = ewi;
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
  input = StringSearch::purify(input);
  for (auto ewi : candidates) {
    if (not ewi.second)
      continue;
    ewi.second->use_cases += 1;
    if (not _cache.contains(ewi.second))
      _cache.append(ewi.second);
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
  for (auto *ex : _cache)
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
