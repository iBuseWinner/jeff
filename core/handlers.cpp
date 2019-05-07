#include "handlers.h"

QString handlers::purify(const QString& l) {
  return handlers::removeCharacters(l.trimmed().toLower());
}

QString handlers::removeCharacters(QString l) {
  for (auto s : ss()) l.remove(s);
  return l;
}
