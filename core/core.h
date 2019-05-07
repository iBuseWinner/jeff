#ifndef CORE_H
#define CORE_H

#include <QDateTime>
#include <QObject>
#include "core/history-processor.h"
#include "core/nlp-module.h"
#include "core/standard-templates.h"

class core : public QObject {
  Q_OBJECT
 public:
  sqlite *sq = new sqlite(this);
  settings *st = new settings(this);
  history_processor *hp = new history_processor(st, this);
  core(QObject *p = nullptr);
  void getUser(QString _ue);
  void getNLP(QString _cn);
  void getWarning(QString _wt);
  void getError(QString _et);
  void getWidget(QWidget *_w);
  message getDaemon(QString _cn, A _a, C _ct, T _t) {
    return d(_cn, _a, _ct, _t);
  }
  // void getScript();

 signals:
  AMessage *show(AMessage *msg);

 private:
  Q_DISABLE_COPY(core)
  NLPmodule *nlp = new NLPmodule(st, sq, this);
  standardTemplates *stdTs = new standardTemplates(this);
  message d(QString _cn, A _a, C _ct, T _t);
  QString curDT() { return QDateTime::currentDateTime().toString(Qt::ISODate); }
};

#endif
