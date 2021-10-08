#ifndef NLPSTRUCTURES_H
#define NLPSTRUCTURES_H

#include "core/source.h"
#include <QList>
#include <QMap>
#include <QString>

/*!
 * @struct SourceRow
 * @brief Contains information about a single row (without address) of a
 * database table.
 */
struct SourceRow {
  /*! Activator. */
  QString activator;
  /*! Reagent address. */
  int reagent_address;
  /*! Reagents properties (name, value). */
  QMap<QString, QString> reagent_properties;
};

/*!
 * @struct LinkMap
 * @brief Contains a map with user expression reagents of a single source.
 */
struct LinkMap {
  /*! Activators and links to their reagents. */
  QMap<QString, QList<int>> activator_links;
  /*! Source data. */
  Source source;
};

/*!
 * @struct GlobalMap
 * @brief Contains a list with activator-reagents pairs.
 */
struct GlobalMap {
  /*! Activators and their reagents. */
  QMap<QString, QStringList> activator_reagents;
};

#endif
