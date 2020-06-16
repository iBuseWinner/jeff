#ifndef NLPMODULE_H
#define NLPMODULE_H

#include "core/basis.h"
#include "core/sqlite.h"
#include "widgets/a_message.h"
#include <QMap>
#include <QObject>
#include <QPair>
#include <QRandomGenerator>
#include <QStringList>
#include <QTime>

/*!
 * Struct: SourceRow.
 * Contains information about a single row [without address] of a database
 * table.
 */
struct SourceRow {
  /*! Activator. */
  QString ac;
  /*! Reagent address. */
  int ra;
  /*! Reagents properties (name, value). */
  QMap<QString, QString> rProps;
};

/*!
 * Struct: LinkMap.
 * Contains a map with user expression reagents of a single container.
 */
struct LinkMap {
  /*! Activators and links to their reagents. */
  QMap<QString, QList<int>> al;
  /*! Container data. */
  Source cProp;
};

/*!
 * Struct: GlobalMap.
 * Contains a list with activator-reagents pairs.
 */
struct GlobalMap {
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
  NLPmodule(Basis *_basis, QObject *parent = nullptr)
      : QObject(parent), basis(_basis) {}
  void search(QString userExpression);

signals:
  QString ready(QString resultExpression);

private:
  // Objects:
  Basis *basis = nullptr;

  // Functions:
  LinkMap toLinkMap(QList<SourceRow> crs, bool _aProp);
  GlobalMap toGlobalMap(const QList<LinkMap> &lms);
  QStringList sorting(const QString &ue, QStringList as);
  void select(QString ue, const GlobalMap &gm);
};

#endif // NLPMODULE_H
