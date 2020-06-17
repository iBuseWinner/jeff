#ifndef NLPMODULE_H
#define NLPMODULE_H

#include "core/basis.h"
#include "core/nlp-structures.h"
#include "core/sqlite.h"
#include "widgets/a_message.h"
#include <QMap>
#include <QObject>
#include <QPair>
#include <QRandomGenerator>
#include <QStringList>
#include <QTime>

/*!
 * Class: NLPmodule.
 * Looks for regular expressions in user input and displays answers to them.
 */
class NLPmodule : public QObject {
  Q_OBJECT
public:
  NLPmodule(Basis *_basis, QObject *parent = nullptr)
      : QObject(parent), basis(_basis) {}
  void search(QString user_expression);

signals:
  QString ready(QString response_expression);

private:
  // Objects:
  Basis *basis = nullptr;

  // Functions:
  LinkMap toLinkMap(QList<SourceRow> source_row_list,
                    bool has_additional_properties);
  GlobalMap toGlobalMap(const QList<LinkMap> &link_map_list);
  QStringList sorting(const QString &user_expression, QStringList activators);
  void select(QString user_expression, const GlobalMap &global_map);
};

#endif // NLPMODULE_H
