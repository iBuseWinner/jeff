#ifndef CORE_H
#define CORE_H

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
  CoreMethods *Meths = new CoreMethods(this);
  historyProcessor *HistoryProcessor = new historyProcessor(Meths, this);

  // Functions:
  core(QObject *parent = nullptr);
  void getUser(QString userExpression);
  void getNLP(QString resultExpression);
  void getWarning(QString warningText);
  void getError(QString errorText);
  void getWidget(QWidget *widget);
  // void getScript();
  void showHistory(QList<message> messageHistory);

  /*! Creates a shadow on demand. */
  message getShadow(QString content, eA author, eC contentType, eT theme) {
    return shadow(content, author, contentType, theme);
  }

 signals:
  AMessage *show(AMessage *message);

 private:
  Q_DISABLE_COPY(core)

  // Objects:
  NLPmodule *nlp = new NLPmodule(Meths, this);
  standardTemplates *stdTs = new standardTemplates(Meths, this);

  // Functions:
  message shadow(QString _cn, eA _a, eC _ct, eT _t);
};

#endif
