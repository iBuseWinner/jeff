#include "nlp-module.h"

void NLPmodule::search_for_suggests(const QString &input) {
  QString purified_input = _basis->sql->purify(input);
  Cache selection = select_from_cache(purified_input);
  if (selection.isEmpty())
    selection = select_from_sql(purified_input);
  if (selection.isEmpty())
    return;
  // #TODO: сделать поиск
  /*!
   * Технология поиска:
   * 1) получить из выборки все выражения
   * 2) получить список индексов вхождения выражений в ввод
   * 3) отсортировать поочерёдно по:
   *    1. индексу (выбрать первый индекс)
   *    2. длине (выбрать наиболее длинное из одинак. индексов)
   *    3. частоте (выбрать при одинаковых длинах)
   *    4. рандомно выбрать, если всё одинаково
   * 4) после этого нужно выражения выбрать и вставить одно за другим
   * 5) обработать
   */
  QMap<int, Expression *> sorted;
  for (auto expr : selection) {
    int key = purified_input.indexOf(_basis->sql->purify(expr->activator_text));
    if (key == -1)
      continue;
    if (sorted.contains(key)) {
      if (_basis->sql->purify(sorted[key]->activator_text).length() <
          _basis->sql->purify(expr->activator_text).length())
        sorted[key] = expr;
      else if (_basis->sql->purify(sorted[key]->activator_text).length() ==
               _basis->sql->purify(expr->activator_text).length()) {
        if (sorted[key]->use_cases < expr->use_cases)
          sorted[key] = expr;
        else if (sorted[key]->use_cases == expr->use_cases) {
          if (_gen->bounded(0, 2) == 1)
            sorted[key] = expr;
        }
      }
    } else
      sorted[key] = expr;
  }
  QString output = QString();
  for (auto expr : sorted) {
    if (purified_input.contains(_basis->sql->purify(expr->activator_text))) {
      expr->use_cases += 1;
      if (not _cache.contains(expr))
        _cache.append(expr);
      output += expr->reagent_text;
      purified_input.replace(_basis->sql->purify(expr->activator_text), "");
    }
  }
  if (not output.isEmpty())
    emit response(output);
}

Cache NLPmodule::select_from_cache(const QString &input) {
  Cache selection;
  for (auto expr : _cache)
    if (input.contains(_basis->sql->purify(expr->activator_text)))
      selection.append(expr);
  return selection;
}

Cache NLPmodule::select_from_sql(const QString &input) {
  Cache selection;
  QList<Source> sources = _basis->get_sources();
  for (const auto &source : sources)
    selection.append(_basis->sql->scan_source(source, input));
  return selection;
}

void NLPmodule::load_cache() {
  _cache = _basis->json->read_NLP_cache(_settings_path);
}

void NLPmodule::save_cache() {
  _basis->json->write_NLP_cache(_cache, _settings_path);
}
