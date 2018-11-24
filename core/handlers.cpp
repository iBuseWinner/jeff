#include "handlers.h"

QString handlers::purifyString(QString line) {
  // Prepares the activator to search by keywords.
  return handlers::removeCharacters(line.trimmed().toLower());
}

QString handlers::removeCharacters(QString line) {
  // Cuts punctuation marks.
  for (int num = 0; num < 8; num++)
    line.remove(symbols[num]);
  return line;
}
