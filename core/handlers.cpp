#include "handlers.h"

/*
 * All short named variables and their explanations:
 * {sb} <- symbol
 */

/*!
 * Argument: QString {str} [string to be purified].
 * Purifies {str}.
 * Returns: purified string.
 */
QString handlers::purify(const QString& str) {
  return handlers::removeCharacters(str.trimmed().toLower());
}

/*!
 * Argument: QString {str}.
 * Removes punctuation.
 * Returns: set of words.
 */
QString handlers::removeCharacters(QString str) {
  for (auto sb : sbs()) str.remove(sb);
  return str;
}
