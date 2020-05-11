#ifndef CORE_H
#define CORE_H

#include <QObject>
#include <QTimer>
#include "core/history-processor.h"
#include "core/nlp-module.h"
#include "core/standard-templates.h"
#include "core/basis.h"

/*!
 * Class: Core.
 * Controls I/O.
 */
class Core : public QObject {
  Q_OBJECT
 public:
  // Objects:
  Basis *basis = new Basis(this);
  HProcessor *historyProcessor = new HProcessor(basis, this);

  // Functions:
  Core(QObject *parent = nullptr);
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
  void setMonologueEnabled(bool enabled) { monologueEnabled = enabled; }

 signals:
  AMessage *show(AMessage *message);

 private:
  Q_DISABLE_COPY(Core)

  // Objects:
  NLPmodule *nlp = new NLPmodule(basis, this);
  StdTemplates *standardTemplates = new StdTemplates(basis, this);
  bool monologueEnabled = false;

  // Functions:
  message shadow(QString _cn, eA _a, eC _ct, eT _t);
};

#endif
