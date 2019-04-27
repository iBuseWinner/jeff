#include "handlers.h"

QString handlers::purifyString(const QString& l) {
  // Prepares the activator to search by keywords.
  return handlers::removeCharacters(l.trimmed().toLower());
}

QString handlers::removeCharacters(QString l) {
  // Cuts punctuation marks.
  for (char s : ss) l.remove(s);
  return l;
}
