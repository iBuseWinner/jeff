#ifndef CORE_H
#define CORE_H

#include <QDateTime>
#include <QObject>
#include <QTimer>
#include "core/history-processor.h"
#include "core/nlp-module.h"
#include "core/standard-templates.h"

/*!
 * Class: core.
 * Controls I/O.
 */
class core : public QObject {
  Q_OBJECT
 public:
  // Objects:
  settings *Settings = new settings(this);
  historyProcessor *HistoryProcessor = new historyProcessor(Settings, this);

  // Functions:
  core(QObject *parent = nullptr);
  void getUser(QString userExpression);
  void getNLP(QString resultExpression);
  void getWarning(QString warningText);
  void getError(QString errorText);
  void getWidget(QWidget *widget);
  // void getScript();

  /*! Creates a shadow on demand. */
  message getShadow(QString content, eA author, eC contentType, eT theme) {
    return shadow(content, author, contentType, theme);
  }

 signals:
  AMessage *show(AMessage *message);

 private:
  Q_DISABLE_COPY(core)

  // Objects:
  NLPmodule *nlp = new NLPmodule(Settings, this);
  standardTemplates *stdTs = new standardTemplates(Settings, this);
  const QString isMonologueEnabledSt = "core/ismonologueenabled";
  const QString isDelayEnabledSt = "core/isdelayenabled";
  const QString minDelaySt = "core/mindelay";
  const QString maxDelaySt = "core/maxdelay";

  // Functions:
  message shadow(QString _cn, eA _a, eC _ct, eT _t);

  /*! Returns a date and time string. */
  QString curDT() { return QDateTime::currentDateTime().toString(Qt::ISODate); }
};

#endif
