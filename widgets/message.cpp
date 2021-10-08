#include "message.h"

/*!
 * @fn Message::Message
 * @brief The constructor.
 * @param[in,out] parent QObject parent
 */
Message::Message() {
  QHBoxLayout *hbox_layout = new QHBoxLayout();
  hbox_layout->setContentsMargins(0, 0, 0, standardMargin);
  hbox_layout->setSpacing(0);
  setLayout(hbox_layout);
}

/*!
 * @fn Message::Message
 * @brief Creates an Message based on @a message.
 * @param[in] message message data
 * @sa MessageData
 */
Message::Message(MessageData message) {
  QHBoxLayout *hbox_layout = new QHBoxLayout();
  hbox_layout->setContentsMargins(0, 0, 0, standardMargin);
  hbox_layout->setSpacing(0);
  setLayout(hbox_layout);
  setMessage(message);
}

/*!
 * @fn Message::setMessage
 * @brief Sets @a _message into the Message.
 * @param[in] _message message data
 * @sa MessageData
 */
void Message::setMessage(MessageData _message) {
  if (_message.datetime.isNull())
    return;
  message = _message;
  setAuthor(message.author);
  setMessageType(message.content_type);
  // setTheme(message.tType);
}

/*!
 * @fn Message::setWidget
 * @brief Sets {@code m_handler->getPrisoner()} into the Message.
 * @param[in,out] m_handler reference to the ModalHandler instance
 */
void Message::setWidget(ModalHandler *m_handler) {
  if (widget)
    return;
  widget = m_handler->getPrisoner();
  connect(m_handler, &ModalHandler::prisonerClosed, this,
          [this] { emit closed(); });
  gridLayout->addWidget(widget);
}

/*!
 * @fn Message::setAuthor
 * @brief Makes a message either left or right.
 * @param[in] aType author of the message
 * @sa Author
 */
void Message::setAuthor(Author aType) {
  switch (aType) {
  case 1:
    setupJeff();
    break;
  case 2:
    setupUser();
    break;
  default:;
  }
}

/*!
 * @fn Message::setMessageType
 * @brief Adjusts the message to the content type.
 * @param[in] cType content type
 * @sa ContentType
 */
void Message::setMessageType(ContentType cType) {
  switch (cType) {
  case 1:
    setupText(message.content);
    break;
  case 2:
    setupMarkdown(message.content);
    break;
  // case 3:
  //  setupPicture(message.content);
  //  break;
  // case 4:
  //  setupFile(message.content);
  //  break;
  case 5:
    setupWarning(message.content);
    break;
  case 6:
    setupError(message.content);
    break;
  case 7:
    prepareSetupWidget();
    return;
  default:;
  }
  alignTextToWindowWidth();
}

/*!
 * @fn Message::setTheme
 * @brief Sets the message colors.
 * @details [constructing]
 * @param[in] tType theme
 */
// void Message::setTheme(Theme tType) {}

/*!
 * @fn Message::setupJeff
 * @brief Customizes layout of message from ASW.
 */
void Message::setupJeff() {
  QPair<QSpacerItem *, Board *> ws = makeLayout();
  layout()->addWidget(ws.second);
  layout()->addItem(ws.first);
}

/*!
 * @fn Message::setupUser
 * @brief Customizes layout of message from user.
 */
void Message::setupUser() {
  QPair<QSpacerItem *, Board *> ws = makeLayout();
  layout()->addItem(ws.first);
  layout()->addWidget(ws.second);
}

/*!
 * @fn Message::setupText
 * @brief Displays plain text.
 * @param[in] content text of the message
 */
void Message::setupText(const QString &content) {
  auto *label = new QLabel(content, this);
  widget = label;
  label->setObjectName("text");
  label->setTextFormat(Qt::PlainText);
  label->setTextInteractionFlags(Qt::TextSelectableByMouse |
                                 Qt::TextSelectableByKeyboard);
  label->setFocusPolicy(Qt::NoFocus);
  gridLayout->addWidget(widget);
}

/*!
 * @fn Message::setupMarkdown
 * @brief Displays markdown text.
 * @param[in] content text of the message
 */
void Message::setupMarkdown(const QString &content) {
  setupText(content);
  auto *label = static_cast<QLabel *>(widget);
  label->setTextFormat(Qt::RichText);
  label->setText(label->text().replace("\n", "<br>"));
}

/*!
 * @fn Message::setupPicture
 * @brief Displays a picture with the path @a content.
 * @details TODO [constructing]
 * @param[in] content path to a picture
 */
// void Message::setupPicture(QString path) {}

/*!
 * @fn Message::setupFile
 * @brief Displays a file with the path @a content.
 * @details TODO [constructing]
 * @param[in] content path to a file
 */
// void Message::setupFile(QString path) {}

/*!
 * @fn Message::setupWarning
 * @brief Displays a warning @a content.
 * \param content text of a warning
 */
void Message::setupWarning(const QString &content) {
  auto *board = static_cast<Board *>(layout()->itemAt(0)->widget());
  board->setStyleSheet(board->warning_style);
  setupText(QString(tr("Warning: ") + content));
}

/*!
 * @fn Message::setupError
 * @brief Displays an error @a content.
 * \param content text of an error
 */
void Message::setupError(const QString &content) {
  auto *board = static_cast<Board *>(layout()->itemAt(0)->widget());
  board->setStyleSheet(board->error_style);
  setupText(QString(tr("Error: ") + content));
}

/*!
 * @fn Message::prepareSetupWidget
 * @brief Prepares Message for widget installation.
 */
void Message::prepareSetupWidget() {
  gridLayout->parentWidget()->setSizePolicy(QSizePolicy::Preferred,
                                            QSizePolicy::Fixed);
}

/*!
 * @fn Message::alignTextToWindowWidth
 * @brief Aligns text to the width of the window.
 */
void Message::alignTextToWindowWidth() {
  auto *label = qobject_cast<QLabel *>(widget);
  if (not label)
    return;
  QTextDocument textDocument;
  if (message.content_type == ContentType::Markdown)
    textDocument.setHtml(label->text());
  else if (message.content_type == ContentType::Text)
    textDocument.setPlainText(label->text());
  else
    return;
  if (textDocument.idealWidth() < maximalMessageWidth)
    label->setWordWrap(false);
  else {
    label->setWordWrap(true);
    label->setFixedWidth(maximalMessageWidth);
  }
}

/*!
 * @fn Message::makeLayout
 * @brief Creates a spacer and an ABoard to adjust the layout.
 * @returns QSpacerItem-ABoard pair
 */
QPair<QSpacerItem *, Board *> Message::makeLayout() {
  auto *spacer = new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding,
                                 QSizePolicy::Minimum);
  auto *board = new Board(this);
  gridLayout = new QGridLayout();
  gridLayout->setSpacing(0);
  gridLayout->setMargin(standardMargin);
  board->setLayout(gridLayout);
  return QPair<QSpacerItem *, Board *>(spacer, board);
}
