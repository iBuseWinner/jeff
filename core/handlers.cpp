#include "handlers.h"

QString handlers::purifyString(const QString& line) {
  // Prepares the activator to search by keywords.
  return handlers::removeCharacters(line.trimmed().toLower());
}

QString handlers::removeCharacters(QString line) {
  // Cuts punctuation marks.
  for (char symbol : symbols)
    line.remove(symbol);
  return line;
}
