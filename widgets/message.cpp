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
Message::Message(MessageData _md) {
  QHBoxLayout *hbox_layout = new QHBoxLayout();
  hbox_layout->setContentsMargins(0, 0, 0, standardMargin);
  hbox_layout->setSpacing(0);
  setLayout(hbox_layout);
  message_data(_md);
}

/*!
 * @fn Message::message_data
 * @brief Sets @a _message_data into the Message.
 * @param[in] _md message data
 * @sa MessageData
 */
void Message::message_data(MessageData _md) {
  if (_md.datetime.isNull()) return;
  md = _md;
  author(md.author);
  content_type(md.content_type);
  // theme(md.theme);
}

/*!
 * @fn Message::widget
 * @brief Sets {modal_handler->getPrisoner()} into the Message.
 * @param[in,out] modal_handler reference to the ModalHandler instance
 */
void Message::widget(ModalHandler *modal_handler) {
  if (w) return;
  w = modal_handler->getPrisoner();
  connect(modal_handler, &ModalHandler::prisonerClosed, this, [this] { emit closed(); });
  grid_layout->addWidget(w);
}

/*!
 * @fn Message::author
 * @brief Makes a message either left or right.
 * @param[in] _a author of the message
 * @sa Author
 */
void Message::author(Author _a) {
  if (_a == Author::Jeff) setup_jeff();
  else setup_user();
}

/*! @brief Updates the text of a message. */
void Message::update_text(const QString &text) {
  auto *label = qobject_cast<QLabel *>(w);
  if (not label) return;
  label->setText(text);
  setMaximumWidth(_width);
}

/*!
 * @fn Message::content_type
 * @brief Adjusts the message to the content type.
 * @param[in] _ct content type
 * @sa ContentType
 * TODO
 */
void Message::content_type(ContentType _ct) {
  if (_ct == ContentType::Text) setup_text(md.content);
  else if (_ct == ContentType::Markdown) setup_markdown(md.content);
  // else if (_ct == ContentType::Picture) setup_picture(md.content);
  // else if (_ct == ContentType::File) setup_file(md.content);
  else if (_ct == ContentType::Warning) setup_warning(md.content);
  else if (_ct == ContentType::Error) setup_error(md.content);
  else prepare_to_widget();
}

/*!
 * @fn Message::theme
 * @brief Sets the message colors.
 * @details [constructing]
 * @param[in] _t theme
 */
// void Message::theme(Theme _t) {}

/*!
 * @fn Message::setup_jeff
 * @brief Customizes layout of message from Jeff.
 */
void Message::setup_jeff() {
  QPair<QSpacerItem *, Board *> ws = make_layout();
  layout()->addWidget(ws.second);
  layout()->addItem(ws.first);
}

/*!
 * @fn Message::setup_user
 * @brief Customizes layout of message from user.
 */
void Message::setup_user() {
  QPair<QSpacerItem *, Board *> ws = make_layout();
  layout()->addItem(ws.first);
  layout()->addWidget(ws.second);
}

/*!
 * @fn Message::setup_text
 * @brief Displays plain text.
 * @param[in] content text of the message
 */
void Message::setup_text(const QString &content) {
  auto *label = new QLabel(content, this);
  w = label;
  label->setObjectName("text");
  label->setTextFormat(Qt::PlainText);
  label->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
  label->setFocusPolicy(Qt::NoFocus);
  grid_layout->addWidget(w);
}

/*!
 * @fn Message::setup_markdown
 * @brief Displays markdown text.
 * @param[in] content text of the message
 */
void Message::setup_markdown(const QString &content) {
  setup_text(content);
  auto *label = static_cast<QLabel *>(w);
  label->setTextFormat(Qt::RichText);
  label->setText(label->text().replace("\n", "<br>"));
}

/*!
 * @fn Message::setup_picture
 * @brief Displays a picture with the path @a content.
 * @details TODO [constructing]
 * @param[in] content path to a picture
 */
// void Message::setup_picture(QString path) {}

/*!
 * @fn Message::setup_file
 * @brief Displays a file with the path @a content.
 * @details TODO [constructing]
 * @param[in] content path to a file
 */
// void Message::setup_file(QString path) {}

/*!
 * @fn Message::setup_warning
 * @brief Displays a warning @a content.
 * @param content text of a warning
 */
void Message::setup_warning(const QString &content) {
  auto *board = static_cast<Board *>(layout()->itemAt(0)->widget());
  board->setStyleSheet(board->warning_style);
  setup_text(QString(tr("Warning: ") + content));
}

/*!
 * @fn Message::setup_error
 * @brief Displays an error @a content.
 * @param content text of an error
 */
void Message::setup_error(const QString &content) {
  auto *board = static_cast<Board *>(layout()->itemAt(0)->widget());
  board->setStyleSheet(board->error_style);
  setup_text(QString(tr("Error: ") + content));
}

/*!
 * @fn Message::prepare_to_widget
 * @brief Prepares Message for widget installation.
 */
void Message::prepare_to_widget() {
  grid_layout->parentWidget()->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
}

/*!
 * @fn Message::make_layout
 * @brief Creates a spacer and an Board to adjust the layout.
 * @returns QSpacerItem-Board pair
 */
QPair<QSpacerItem *, Board *> Message::make_layout() {
  auto *spacer = new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);
  auto *board = new Board(this);
  grid_layout = new QGridLayout();
  grid_layout->setSpacing(0);
  grid_layout->setMargin(standardMargin);
  board->setLayout(grid_layout);
  return QPair<QSpacerItem *, Board *>(spacer, board);
}

/*! @brief Fits the text to the size of the message. */
void Message::setWidth(int width) {
  auto *label = qobject_cast<QLabel *>(w);
  if (not label) {
    if (w) w->setMaximumWidth(width);
    return;
  }
  QTextDocument textDocument;
  if (md.content_type == ContentType::Markdown)
    textDocument.setHtml(label->text());
  else if (md.content_type == ContentType::Text)
    textDocument.setPlainText(label->text());
  else
    return;
  if (textDocument.idealWidth() < width)
    label->setWordWrap(false);
  else {
    label->setWordWrap(true);
    label->setFixedWidth(width);
  }
  _width = width;
}
