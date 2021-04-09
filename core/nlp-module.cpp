#include "nlp-module.h"

void

void NLPmodule::search_for_suggests(const QString &input) {
  QString purified_input = _basis->sql->purify(input);
  Cache selection = select_from_cache(purified_input);
  if (selection.isEmpty()) selection = select_from_sql(purified_input);
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
