#ifndef NLPMODULE_H
#define NLPMODULE_H

#include <QMap>
#include <QObject>
#include <QPair>
#include <QRandomGenerator>
#include <QStringList>
#include <QTime>
#include "core/settings.h"
#include "core/sqlite.h"
#include "widgets/a_message.h"

struct containerRow {
  QString a;                      // activator
  int ra;                         // reagent address
  QMap<QString, QString> rProps;  // reagents properties (name, value)
};

struct linkMap {
  QMap<QString, QList<int>> rl;  // reagents and their links
  container cProp;               // container data
};

struct globalMap {
  QMap<QString, QStringList> ar;  // activators and their reagents
};

class NLPmodule : public QObject {
  Q_OBJECT
 public:
  NLPmodule(settings *_st, sqlite *_sq, QObject *p = nullptr);
  void search(QString ue);

 signals:
  QString ready(QString rs);

 private:
  sqlite *sq = nullptr;
  settings *st = nullptr;
  linkMap toLinkMap(const QList<containerRow> &cm, bool _aProp);
  globalMap toGlobalMap(const QList<linkMap> &sRes);
  QStringList sorting(const QString &ue, QStringList ks);
  void select(QString ue, const globalMap &genm);
  QString simplifier(const QString &ex);
};

#endif  // NLPMODULE_H
