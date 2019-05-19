#ifndef NLPMODULE_H
#define NLPMODULE_H

#include <QMap>
#include <QObject>
#include <QPair>
#include <QRandomGenerator>
#include <QStringList>
#include <QTime>
#include "core/core-methods.h"
#include "core/sqlite.h"
#include "widgets/a_message.h"

/*!
 * Struct: containerRow.
 * Contains information about a single row [without address] of a database
 * table.
 */
struct containerRow {
  /*! Activator. */
  QString ac;
  /*! Reagent address. */
  int ra;
  /*! Reagents properties (name, value). */
  QMap<QString, QString> rProps;
};

/*!
 * Struct: linkMap.
 * Contains a map with user expression reagents of a single container.
 */
struct linkMap {
  /*! Activators and links to their reagents. */
  QMap<QString, QList<int>> al;
  /*! Container data. */
  container cProp;
};

/*!
 * Struct: globalMap.
 * Contains a list with activator-reagents pairs.
 */
struct globalMap {
  /*! Activators and their reagents. */
  QMap<QString, QStringList> ars;
};

/*!
 * Class: NLPmodule.
 * Looks for regular expressions in user input and displays answers to them.
 */
class NLPmodule : public QObject {
  Q_OBJECT
 public:
  NLPmodule(CoreMethods *_Meths, QObject *parent = nullptr);
  void search(QString userExpression);

 signals:
  QString ready(QString resultExpression);

 private:
  // Objects:
  CoreMethods *Meths = nullptr;

  // Functions:
  linkMap toLinkMap(QList<containerRow> crs, bool _aProp);
  globalMap toGlobalMap(const QList<linkMap> &lms);
  QStringList sorting(const QString &ue, QStringList as);
  void select(QString ue, const globalMap &gm);
};

#endif  // NLPMODULE_H
