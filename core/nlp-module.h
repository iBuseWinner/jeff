#ifndef NLPMODULE_H
#define NLPMODULE_H

#include <QMap>
#include <QObject>
#include <QPair>
#include <QRandomGenerator>
#include <QStringList>
#include <QTime>
#include "core/containersstruct.h"
#include "core/handlers.h"
#include "core/settingsstore.h"
#include "core/sqlite.h"
#include "core/standardtemplates.h"
#include "widgets/a_message.h"

struct containerRow {
  QString a;
  int rAdd;
  QMap<QString, QString> rProps;
};

struct linksMicroMap {
  QMap<QString, QList<int>> el;
  containerProperties pr;
};

struct globalExpressionNetworkMap {
  QMap<QString, QStringList> ar;
};

class NLPmodule : public QObject {
  Q_OBJECT
 public:
  AMessage *get(const QString &ue, AMessage::A a, QWidget *p = nullptr);
  QString think(const QString &ue /*, AkiwakeMessage *msg*/);
  QString preparePlugins(QString ue, AMessage *msg);
  linksMicroMap handlePlugins(const QList<containerRow>& cm,
                              bool _aProp/*,
                              AkiwakeMessage *msg*/);
  globalExpressionNetworkMap microMapCombinator(
      sqlite *sq, const QList<linksMicroMap> &sRes);
  QStringList sorting(const QString &ue, QStringList ks);
  QString selectReagents(QString ue, const globalExpressionNetworkMap &genm);

 private:
  Q_DISABLE_COPY(NLPmodule)
  QString simplifier(const QString &ex);
};

#endif  // NLPMODULE_H
