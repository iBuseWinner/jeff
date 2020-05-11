#include "core.h"

/*
 * All short named objects and their explanations:
 * {basis} <- core methods
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
Core::Core(QObject *parent) : QObject(parent) {
  connect(basis, &Basis::jsonError, this, &Core::getError);
  connect(basis, &Basis::settingsWarning, this, &Core::getWarning);
  connect(historyProcessor, &HProcessor::sendMessageHistory, this,
          &Core::showHistory);
  connect(basis->sql, &SQLite::sqliteError, this, &Core::getError);
  connect(basis->sql, &SQLite::sqliteWarning, this, &Core::getWarning);
  connect(standardTemplates, &StdTemplates::showASWDialog, this,
          &Core::getWidget);
  connect(nlp, &NLPmodule::ready, this, &Core::getNLP);
  connect(standardTemplates, &StdTemplates::changeMonologueMode, this,
          [this] { setMonologueEnabled(!monologueEnabled); });
  setMonologueEnabled(basis->read(basis->isMonologueModeEnabledSt).toBool());
}

/*!
 * Argument: QString {userExpression} [contains user input].
 * Handles input {userExpression}, displays a message on the screen and launches
 * modules.
 */
void Core::getUser(QString userExpression) {
  // Does not respond to blank input.
  if (userExpression.isEmpty())
    return;
  // Displays the entered message on the screen.
  Message sh =
      shadow(userExpression, Author::User, ContentType::Markdown, Theme::Std);
  historyProcessor->append(sh);
  emit show(new AMessage(sh));
  // If a user has entered the command, there is no need to run other modules.
  if (standardTemplates->dialogues(userExpression))
    return;
  if (standardTemplates->fastCommands(userExpression))
    return;
  nlp->search(userExpression);
}

/*!
 * Argument: QString {resultExpression} [contains the response of the NLP module
 * to input].
 * Processes the output of the NLP module {resultExpression} and displays a
 * message on the screen.
 */
void Core::getNLP(QString resultExpression) {
  if (resultExpression.isEmpty())
    return;
  Message sh =
      shadow(resultExpression, Author::ASW, ContentType::Markdown, Theme::Std);
  historyProcessor->append(sh);
  QTimer::singleShot(
      basis->read(basis->isDelayEnabledSt).toBool()
          ? QRandomGenerator().bounded(basis->read(basis->minDelaySt).toInt(),
                                       basis->read(basis->maxDelaySt).toInt())
          : 0,
      this, [this, sh, resultExpression] {
        emit show(new AMessage(sh));
        if (monologueEnabled)
          nlp->search(resultExpression);
      });
}

/*!
 * Argument: QString {warningText} [contains the warning text of some module].
 * Displays {warningText}.
 */
void Core::getWarning(QString warningText) {
  // The warning color is yellow.
  Message sh =
      shadow(warningText, Author::ASW, ContentType::Warning, Theme::Yellow);
  historyProcessor->append(sh);
  emit show(new AMessage(sh));
}

/*!
 * Argument: QString {errorText} [contains the error text of some module].
 * Displays {errorText}.
 */
void Core::getError(QString errorText) {
  // The error color is red.
  Message sh = shadow(errorText, Author::ASW, ContentType::Error, Theme::Red);
  historyProcessor->append(sh);
  emit show(new AMessage(sh));
}

/*!
 * Argument: QWidget {*widget} [widget that should be displayed].
 * Creates AMessage {*msg}, inserts a widget into it and displays a message.
 */
void Core::getWidget(QWidget *widget) {
  Message sh = shadow(widget->objectName(), Author::ASW, ContentType::Widget,
                      Theme::Std);
  historyProcessor->append(sh);
  auto *msg = new AMessage(sh);
  widget->setParent(msg);
  widget->setFixedWidth(400);
  msg->setWidget(widget);
  emit show(msg);
}

/*!
 * Argument: QList of messages {messageHistory}.
 * Displays all messages from {messageHistory} on the screen.
 */
void Core::showHistory(QList<Message> messageHistory) {
  for (auto shadow : messageHistory)
    emit show(new AMessage(shadow));
}

/*!
 * Arguments: QString {_cn} [shadow content],
 *            enum eA {_a} [who is author],
 *            enum eC {_ct} [content type],
 *            enum eT {_t} [appearance of the message].
 * Creates a shadow of a future AMessage.
 */
Message Core::shadow(QString _cn, Author _a, ContentType _ct, Theme _t) {
  Message _sh;
  _sh.content = _cn;
  _sh.datetime = QDateTime::currentDateTime();
  _sh.aType = _a;
  _sh.cType = _ct;
  _sh.tType = _t;
  return _sh;
}

void Core::setMonologueEnabled(bool enabled) {
  monologueEnabled = enabled;
  emit changeMenuBarMonologueCheckbox(enabled);
}
