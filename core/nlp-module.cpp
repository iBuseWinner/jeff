#include "nlp-module.h"

void NLPmodule::search_for_suggests(const QString &input) {
  QString purified_input = _basis->sql->purify(input);
  Cache selection = select_from_cache(purified_input);
  if (selection.isEmpty()) selection = select_from_sql(purified_input);
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

}

Cache NLPmodule::select_from_cache(const QString &input) {
  Cache selection;
  for (auto expr : _cache) {
    if (input.contains(
            _basis->sql->purify(expr.activator_text))) {
      selection.append(expr);
    }
  }
  return selection;
}

Cache NLPmodule::select_from_sql(const QString &input) {
  Cache selection;
  QList<Source> sources = _basis->get_sources();
  for (auto source : sources) {
    selection.append(_basis->sql->scan_source(source, input));
  }
  return selection;
}

void NLPmodule::load_cache() {}

void NLPmodule::save_cache() {}
