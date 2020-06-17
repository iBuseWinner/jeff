#include "core.h"

/*
 * All short named objects and their explanations:
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
  basis->check();
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
  Message message = formMessage(userExpression, Author::User,
                                ContentType::Markdown, Theme::Std);
  historyProcessor->append(message);
  emit show(new AMessage(message));
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
  Message message = formMessage(resultExpression, Author::ASW,
                                ContentType::Markdown, Theme::Std);
  historyProcessor->append(message);
  QTimer::singleShot(
      basis->read(basis->isDelayEnabledSt).toBool()
          ? QRandomGenerator().bounded(basis->read(basis->minDelaySt).toInt(),
                                       basis->read(basis->maxDelaySt).toInt())
          : 0,
      this, [this, message, resultExpression] {
        emit show(new AMessage(message));
        if (monologueEnabled)
          nlp->search(resultExpression);
      });
}

/*!
 * Argument: QString {warningText} [contains the warning text of some module].
 * Displays {warningText}.
 */
void Core::getWarning(const QString &warningText) {
  // The warning color is yellow.
  Message message = formMessage(warningText, Author::ASW, ContentType::Warning,
                                Theme::Yellow);
  historyProcessor->append(message);
  emit show(new AMessage(message));
}

/*!
 * Argument: QString {errorText} [contains the error text of some module].
 * Displays {errorText}.
 */
void Core::getError(const QString &errorText) {
  // The error color is red.
  Message message =
      formMessage(errorText, Author::ASW, ContentType::Error, Theme::Red);
  historyProcessor->append(message);
  emit show(new AMessage(message));
}

/*!
 * Argument: QWidget {*widget} [widget that should be displayed].
 * Creates AMessage {*message_widget}, inserts a widget into it and displays a
 * message.
 */
void Core::getWidget(QWidget *widget) {
  Message message = formMessage(widget->objectName(), Author::ASW,
                                ContentType::Widget, Theme::Std);
  historyProcessor->append(message);
  auto *message_widget = new AMessage(message);
  widget->setParent(message_widget);
  widget->setFixedWidth(400);
  message_widget->setWidget(widget);
  emit show(message_widget);
}

/*!
 * Argument: QList of messages {message_history}.
 * Displays all messages from {message_history} on the screen.
 */
void Core::showHistory(QList<Message> message_history) {
  for (const auto &message : message_history)
    emit show(new AMessage(message));
}

/*!
 * Arguments: QString {_cn} [shadow content],
 *            enum eA {_a} [who is author],
 *            enum eC {_ct} [content type],
 *            enum eT {_t} [appearance of the message].
 * Creates a shadow of a future AMessage.
 */
Message Core::formMessage(const QString &_cn, Author _a, ContentType _ct,
                          Theme _t) {
  Message _message;
  _message.content = _cn;
  _message.datetime = QDateTime::currentDateTime();
  _message.aType = _a;
  _message.cType = _ct;
  _message.tType = _t;
  return _message;
}

void Core::setMonologueEnabled(bool enabled) {
  monologueEnabled = enabled;
  emit changeMenuBarMonologueCheckbox(enabled);
}
