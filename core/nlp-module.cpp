#include "nlp-module.h"

/*!
 * @fn NLPmodule::select_candidates
 * @brief Matches candidates for each index of occurrence in the input
 * expression.
 * @param[in] selection selected expressions
 * @param[in] input text string
 * @returns dictionary with indices and selected expressions
 */
QMap<int, Expression *> NLPmodule::select_candidates(Cache selection,
                                                     QString input) {
  QMap<int, Expression *> candidates;
  for (auto *expr : selection) {
    auto x = StringSearch::contains(input, expr->activator_text);
    if (x.first == 0 and x.second == 0)
      continue;
    if (candidates.contains(x.first)) {
      if (candidates[x.first]->activator_text.length() <
          expr->activator_text.length())
        candidates[x.first] = expr;
      else if (candidates[x.first]->activator_text.length() ==
               expr->activator_text.length()) {
        if (candidates[x.first]->use_cases < expr->use_cases)
          candidates[x.first] = expr;
        else if (candidates[x.first]->use_cases == expr->use_cases) {
          if (_gen->bounded(0, 2) == 1)
            candidates[x.first] = expr;
        }
      }
    } else
      candidates[x.first] = expr;
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
QPair<QString, QString>
NLPmodule::compose_answer(QString input, QMap<int, Expression *> candidates) {
  QString output;
  std::cout << "--------" << std::endl;
  std::cout << "\tfn compose_answer" << std::endl;
  std::cout << "\tGot input as \"" << input.toStdString() << "\"" << std::endl;
  for (auto i : candidates.keys()) {
    std::cout << "\tHmm..." << std::endl;
    std::cout << "\tWhat, if we haven't candidates?" << std::endl;
    if (not candidates[i]) {
      std::cout << "\tWWW" << std::endl;
      continue;
    }
    std::cout << "\tHere we go again. Let's try:" << std::endl;
    auto x = StringSearch::contains(
        input, candidates[i]->activator_text);
    if (not(x.first == 0 and x.second == 0)) {
      std::cout << "\tSo, found x.first = " << x.first
                << " and x.second = " << x.second
                << ", and we found it in input" << std::endl;
      candidates[i]->use_cases += 1;
      if (not _cache.contains(candidates[i]))
        _cache.append(candidates[i]);
      output += candidates[i]->reagent_text + " ";
      input = StringSearch::replace(input, x);
    }
  }
  return QPair<QString, QString>(input, output);
}

/*!
 * @fn NLPmodule::search_for_suggests
 * @brief Matches the answer based on the input.
 * @param[in] input user input
 */
void NLPmodule::search_for_suggests(const QString &input) {
  Cache selection = select_from_cache(input);
  bool from_db = false;
  if (selection.isEmpty()) {
    selection = select_from_db(input);
    from_db = true;
  }
  std::cout << "--------" << std::endl;
  std::cout << "\tfn search_for_suggests" << std::endl;
  std::cout << "\tGot input as \"" << input.toStdString() << "\"" << std::endl;
  std::cout << "\tCalculated selection.length() = " << selection.length()
            << std::endl;
  std::cout << "\tSelection is from DB? " << from_db << std::endl;
  if (selection.isEmpty())
    return;
  auto sorted = select_candidates(selection, input);
  auto composition = compose_answer(input, sorted);
  if (composition.first.length() / input.length() > 0.33 and not from_db) {
    selection.clear();
    selection = select_from_db(input);
    if (selection.isEmpty())
      return;
    sorted.clear();
    sorted = select_candidates(selection, input);
    composition = compose_answer(input, sorted);
  }
  std::cout << "--------" << std::endl;
  std::cout << "\tfn search_for_suggests (requested)" << std::endl;
  std::cout << "\tCalculated response: \"" << composition.second.toStdString()
            << "\"" << std::endl;
  emit response(composition.second);
}

/*!
 * @fn NLPmodule::select_from_cache
 * @brief Selects expressions to compose a response from the cache.
 * @param[in] input user input
 * @returns matching expressions
 */
Cache NLPmodule::select_from_cache(const QString &input) {
  Cache selection;
  std::cout << "--------" << std::endl;
  std::cout << "\tfn select_from_cache" << std::endl;
  std::cout << "\tCache:" << std::endl;
  for (auto *ex : _cache)
    std::cout << "\t  \"" << ex->activator_text.toStdString() << "\"" << std::endl;
  for (int i = 0; i < _cache.length(); i++) {
    auto x = StringSearch::contains(input, _cache[i]->activator_text);
    if (not(x.first == 0 and x.second == 0))
      selection.append(_cache[i]);
  }
  return selection;
}

/*!
 * @fn NLPmodule::select_from_db
 * @brief Selects expressions to compose a response from the database.
 * @param[in] input user input
 * @returns matching expressions
 */
Cache NLPmodule::select_from_db(const QString &input) {
  Cache selection;
  Sources sources = _basis->get_sources();
  for (int i = 0; i < sources.length(); i++)
    selection.append(_basis->sql->scan_source(sources[i], input));
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
