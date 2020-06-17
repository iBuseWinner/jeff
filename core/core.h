#ifndef CORE_H
#define CORE_H

#include "core/basis.h"
#include "core/history-processor.h"
#include "core/nlp-module.h"
#include "core/standard-templates.h"
#include "dialogues/modal-handler.h"
#include <QObject>
#include <QTimer>

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
  void getWarning(const QString &warningText);
  void getError(const QString &errorText);
  void getWidget(ModalHandler *m_handler);
  // void getScript();
  void showHistory(QList<Message> message_history);

  /*! Creates a shadow on demand. */
  Message getMessage(QString content, Author author, ContentType contentType,
                     Theme theme) {
    return formMessage(content, author, contentType, theme);
  }
  void setMonologueEnabled(bool enabled);

signals:
  AMessage *show(AMessage *message);
  bool changeMenuBarMonologueCheckbox(bool enabled);

private:
  Q_DISABLE_COPY(Core)

  // Objects:
  NLPmodule *nlp = new NLPmodule(basis, this);
  StdTemplates *standardTemplates = new StdTemplates(basis, this);
  bool monologueEnabled = false;

  // Functions:
  Message formMessage(const QString &_cn, Author _a, ContentType _ct, Theme _t);
};

#endif
