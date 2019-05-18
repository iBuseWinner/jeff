#include "core.h"

/*
 * All short named objects and their explanations:
 * {msg} <- message
 * {nlp} <- NLPmodule
 * {stdTs} <- standard templates
 * {_sh} <- shadow
 * {_cn} <- content
 * {_a} <- author
 * {_ct} <- content type
 * {_t} <- theme
 */

/*!
 * Argument: QObject {*parent}.
 * Creates connections between modules and output collectors.
 */
core::core(QObject *parent) : QObject(parent) {
  connect(Settings, &settings::jsonError, this, &core::getError);
  connect(Settings, &settings::settingsWarning, this, &core::getWarning);
  connect(Settings->SQL, &sqlite::sqliteError, this, &core::getError);
  connect(Settings->SQL, &sqlite::sqliteWarning, this, &core::getWarning);
  connect(stdTs, &standardTemplates::showASWDialog, this, &core::getWidget);
  connect(nlp, &NLPmodule::ready, this, &core::getNLP);
}

/*!
 * Argument: QString {userExpression} [contains user input].
 * Handles input {userExpression}, displays a message on the screen and launches
 * modules.
 */
void core::getUser(QString userExpression) {
  // Does not respond to blank input.
  if (userExpression == "") return;
  // Displays the entered message on the screen.
  emit show(
      new AMessage(shadow(userExpression, eA::User, eC::Markdown, eT::Std)));
  // If a user has entered the command, there is no need to run other modules.
  if (stdTs->dialogues(userExpression)) return;
  nlp->search(userExpression);
}

/*!
 * Argument: QString {resultExpression} [contains the response of the NLP module
 * to input].
 * Processes the output of the NLP module {resultExpression} and displays a
 * message on the screen.
 */
void core::getNLP(QString resultExpression) {
  if (resultExpression == "") return;
  QTimer::singleShot(
      Settings->read(isDelayEnabledSt).toBool()
          ? QRandomGenerator().bounded(Settings->read(minDelaySt).toInt(),
                                       Settings->read(maxDelaySt).toInt())
          : 0,
      this, [this, resultExpression] {
        emit show(new AMessage(
            shadow(resultExpression, eA::ASW, eC::Markdown, eT::Std)));
      });
  if (Settings->read(isMonologueEnabledSt).toBool())
    nlp->search(resultExpression);
}

/*!
 * Argument: QString {warningText} [contains the warning text of some module].
 * Displays {warningText}.
 */
void core::getWarning(QString warningText) {
  // The warning color is yellow.
  emit show(
      new AMessage(shadow(warningText, eA::ASW, eC::Warning, eT::Yellow)));
}

/*!
 * Argument: QString {errorText} [contains the error text of some module].
 * Displays {errorText}.
 */
void core::getError(QString errorText) {
  // The error color is red.
  emit show(new AMessage(shadow(errorText, eA::ASW, eC::Error, eT::Red)));
}

/*!
 * Argument: QWidget {*widget} [widget that should be displayed].
 * Creates AMessage {*msg}, inserts a widget into it and displays a message.
 */
void core::getWidget(QWidget *widget) {
  auto *msg =
      new AMessage(shadow(widget->objectName(), eA::ASW, eC::Widget, eT::Std));
  widget->setParent(msg);
  widget->setFixedWidth(400);
  msg->setWidget(widget);
  emit show(msg);
}

/*!
 * Arguments: QString {_cn} [shadow content],
 *            enum eA {_a} [who is author],
 *            enum eC {_ct} [content type],
 *            enum eT {_t} [appearance of the message].
 * Creates a shadow of a future AMessage.
 */
message core::shadow(QString _cn, eA _a, eC _ct, eT _t) {
  message _sh;
  _sh.content = _cn;
  _sh.datetime = curDT();
  _sh.aType = _a;
  _sh.cType = _ct;
  _sh.tType = _t;
  return _sh;
}
