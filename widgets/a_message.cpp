#include "a_message.h"

/*!
 * @fn AMessage::AMessage
 * @brief The constructor.
 * @param[in,out] parent QObject parent
 */
AMessage::AMessage() {
  QHBoxLayout *hbox_layout = new QHBoxLayout();
  hbox_layout->setContentsMargins(0, 0, 0, standardMargin);
  hbox_layout->setSpacing(0);
  setLayout(hbox_layout);
}

/*!
 * @fn AMessage::AMessage
 * @brief Creates an AMessage based on @a message.
 * @param[in] message message data
 */
AMessage::AMessage(Message message) {
  QHBoxLayout *hbox_layout = new QHBoxLayout();
  hbox_layout->setContentsMargins(0, 0, 0, standardMargin);
  hbox_layout->setSpacing(0);
  setLayout(hbox_layout);
  setMessage(message);
}

/*!
 * @fn AMessage::setMessage
 * @brief Sets @a _message into the AMessage.
 * @param[in] _message message data
 */
void AMessage::setMessage(Message _message) {
  if (_message.datetime.isNull())
    return;
  message = _message;
  setAuthor(message.author);
  setMessageType(message.content_type);
  // setTheme(message.tType);
}

/*!
 * @fn AMessage::setWidget
 * @brief Sets {@code m_handler->getPrisoner()} into the AMessage.
 * @param[in,out] m_handler reference to the ModalHandler instance
 */
void AMessage::setWidget(ModalHandler *m_handler) {
  if (widget)
    return;
  widget = m_handler->getPrisoner();
  connect(m_handler, &ModalHandler::prisonerClosed, this,
          [this] { emit closed(); });
  gridLayout->addWidget(widget);
}

/*!
 * @fn AMessage::setAuthor
 * @brief Makes a message either left or right.
 * @param[in] aType author of the message
 */
void AMessage::setAuthor(Author aType) {
  switch (aType) {
  case 1:
    setupASW();
    break;
  case 2:
    setupUser();
    break;
  default:;
  }
}

/*!
 * @fn AMessage::setMessageType
 * @brief Adjusts the message to the content type.
 * @param[in] cType content type
 * @sa ContentType
 */
void AMessage::setMessageType(ContentType cType) {
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
 * @fn AMessage::setTheme
 * @brief Sets the message colors.
 * @details [constructing]
 * @param[in] tType theme
 */
// void AMessage::setTheme(Theme tType) {}

/*!
 * @fn AMessage::setupASW
 * @brief Customizes layout of message from ASW.
 */
void AMessage::setupASW() {
  QPair<QSpacerItem *, ABoard *> ws = makeLayout();
  layout()->addWidget(ws.second);
  layout()->addItem(ws.first);
}

/*!
 * @fn AMessage::setupUser
 * @brief Customizes layout of message from user.
 */
void AMessage::setupUser() {
  QPair<QSpacerItem *, ABoard *> ws = makeLayout();
  layout()->addItem(ws.first);
  layout()->addWidget(ws.second);
}

/*!
 * @fn AMessage::setupText
 * @brief Displays plain text.
 * @param[in] content text of the message
 */
void AMessage::setupText(const QString &content) {
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
 * @fn AMessage::setupMarkdown
 * @brief Displays markdown text.
 * @param[in] content text of the message
 */
void AMessage::setupMarkdown(const QString &content) {
  setupText(content);
  auto *label = static_cast<QLabel *>(widget);
  label->setTextFormat(Qt::RichText);
  label->setText(label->text().replace("\n", "<br>"));
}

/*!
 * @fn AMessage::setupPicture
 * @brief Displays a picture with the path @a content.
 * @details [constructing]
 * @param[in] content path to a picture
 */
// void AMessage::setupPicture(QString path) {}

/*!
 * @fn AMessage::setupFile
 * @brief Displays a file with the path @a content.
 * @details [constructing]
 * @param[in] content path to a file
 */
// void AMessage::setupFile(QString path) {}

/*!
 * @fn AMessage::setupWarning
 * @brief Displays a warning @a content.
 * \param content text of a warning
 */
void AMessage::setupWarning(const QString &content) {
  auto *board = static_cast<ABoard *>(layout()->itemAt(0)->widget());
  board->setStyleSheet(board->warning_style);
  setupText(QString("Warning: " + content));
}

/*!
 * @fn AMessage::setupError
 * @brief Displays an error @a content.
 * \param content text of an error
 */
void AMessage::setupError(const QString &content) {
  auto *board = static_cast<ABoard *>(layout()->itemAt(0)->widget());
  board->setStyleSheet(board->error_style);
  setupText(QString("Error: " + content));
}

/*!
 * @fn AMessage::prepareSetupWidget
 * @brief Prepares AMessage for widget installation.
 */
void AMessage::prepareSetupWidget() {
  gridLayout->parentWidget()->setSizePolicy(QSizePolicy::Preferred,
                                            QSizePolicy::Fixed);
}

/*!
 * @fn AMessage::alignTextToWindowWidth
 * @brief Aligns text to the width of the window.
 */
void AMessage::alignTextToWindowWidth() {
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
 * @fn AMessage::makeLayout
 * @brief Creates a spacer and an ABoard to adjust the layout.
 * @returns QSpacerItem-ABoard pair
 */
QPair<QSpacerItem *, ABoard *> AMessage::makeLayout() {
  auto *spacer = new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding,
                                 QSizePolicy::Minimum);
  auto *board = new ABoard(this);
  gridLayout = new QGridLayout();
  gridLayout->setSpacing(0);
  gridLayout->setMargin(standardMargin);
  board->setLayout(gridLayout);
  return QPair<QSpacerItem *, ABoard *>(spacer, board);
}
