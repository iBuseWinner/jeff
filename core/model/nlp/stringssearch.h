#ifndef STRINGSSEARCH_H
#define STRINGSSEARCH_H

#include <QList>
#include <QMap>
#include <QPair>
#include <QString>
#include <iostream>

/*!
 * @struct WordMetadata
 * @brief Stores a word and indexes positioning it in a string.
 * @sa StringSearch::contains
 */
struct WordMetadata {
  int i1 = 0;
  int i2 = 0;
  QString word;
};

/*!
 * @enum Intersects
 * @brief Enumerating options for @a intersects.
 * @sa StringSearch::intersects
 */
enum Intersects { No, FirstBetter, SecondBetter, Equal };

/*!
 * @class StringSearch
 * @brief Contains methods for working with user input.
 */
class StringSearch {
public:
  // Functions described in 'stringssearch.cpp':
  static QString remove_symbols(QString str);
  static QString purify(const QString &str);
  static float get_POC(const QString &e1, const QString &e2);
  static QMap<int, int> contains(QString that, QString inner, float EL = 0.66);
  static QPair<Intersects, int> intersects(QMap<int, int> first,
                                           QMap<int, int> second);
  static QString replace(QString that, QMap<int, int> indices, QString to = "");
};

#endif
