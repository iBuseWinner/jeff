#ifndef STRINGSSEARCH_H
#define STRINGSSEARCH_H

#include "yelloger.h"
#include <QList>
#include <QMap>
#include <QPair>
#include <QString>

/*! @namespace StringSearch
 *  @brief Contains methods for working with user input.  */
namespace StringSearch {
  /*! @struct WordMetadata
   *  @brief Stores a word and indexes positioning it in a string.
   *  @sa StringSearch::contains  */
  struct WordMetadata {
    int i1 = 0;
    int i2 = 0;
    QString word;
  };

  /*! @enum Intersects
   *  @brief Enumerating options for @a intersects.
   *  @sa StringSearch::intersects  */
  enum Intersects { No, FirstBetter, SecondBetter, Equal };

  // Functions described in `stringssearch.cpp`:
  QMap<int, int> contains(QString that, QString inner, float EL = 0.66, bool HA = false);
  QString remove_symbols(QString str);
  QString lemmatize(const QString &str);
  float get_POC(const QString &e1, const QString &e2);
  QPair<Intersects, int> intersects(QMap<int, int> first, QMap<int, int> second);
  QString replace(QString that, QMap<int, int> indices, QString to = "");
  QString locate(const QString &that, int last_index);
}

#endif
