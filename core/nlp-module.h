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
 * @class NLPmodule
 * @brief Looks for regular expressions in user input and displays answers to
 * them.
 * @sa Basis
 */
class NLPmodule : public QObject {
  Q_OBJECT
public:
  // Functions:
  /*! The constructor. */
  NLPmodule(Basis *basis, QObject *parent = nullptr)
      : QObject(parent), _basis(basis) {}

  // Functions described in `nlp-module.cpp`:
  void search(QString user_expression);

signals:
  QString ready(QString response_expression);

private:
  // Objects:
  Basis *_basis = nullptr;

  // Functions described in `nlp-module.cpp`:
  LinkMap to_link_map(QList<SourceRow> source_row_list,
                    bool has_additional_properties);
  GlobalMap to_global_map(const QList<LinkMap> &link_map_list);
  QStringList sorting(const QString &user_expression, QStringList activators);
  void select(QString user_expression, const GlobalMap &global_map);
};

#endif // NLPMODULE_H
