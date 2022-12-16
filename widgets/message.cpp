#include "message.h"

/*! @brief The constructor. */
Message::Message() {
  QHBoxLayout *hbox_layout = new QHBoxLayout();
  hbox_layout->setContentsMargins(0, 0, 0, standardMargin);
  hbox_layout->setSpacing(0);
  setLayout(hbox_layout);
}

/*! @brief Creates a Message based on @a message. */
Message::Message(MessageData _md) {
  QHBoxLayout *hbox_layout = new QHBoxLayout();
  hbox_layout->setContentsMargins(0, 0, 0, standardMargin);
  hbox_layout->setSpacing(0);
  setLayout(hbox_layout);
  message_data(_md);
}

/*! @brief Sets @a _message_data into the Message. */
void Message::message_data(MessageData _md) {
  if (_md.datetime.isNull()) return;
  md = _md;
  author(md.author);
  content_type(md.content_type);
  theme(md.theme);
}

/*! @brief Sets {modal_handler->getPrisoner()} into the Message. */
void Message::widget(ModalHandler *modal_handler) {
  if (w) return;
  w = modal_handler->getPrisoner();
  connect(modal_handler, &ModalHandler::prisonerClosed, this, [this] { emit closed(); });
  grid_layout->addWidget(w);
}

/*! @brief Makes a message either left or right. */
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

/*! @brief Adjusts the message to the content type. */
void Message::content_type(ContentType _ct) {
  if (_ct == ContentType::Picture or 
      md.content.toLower().endsWith(".jpg") or 
      md.content.toLower().endsWith(".png")) {
    auto info = QFileInfo(md.content);
    if (info.exists() and info.isFile()) {
      md.content_type = ContentType::Picture;
      setup_picture(md.content);
    } else setup_text(md.content);
  }
  else if (_ct == ContentType::Text) setup_text(md.content);
  else if (_ct == ContentType::Markdown) setup_markdown(md.content);
  // else if (_ct == ContentType::File) setup_file(md.content);
  else if (_ct == ContentType::Warning) setup_warning(md.content);
  else if (_ct == ContentType::Error) setup_error(md.content);
  else prepare_to_widget();
}

/*! @brief Sets the message colors. */
void Message::theme(Theme _t) {
  if (_t == Theme::Std) return; /*!< This theme is by default, and message theme 
                                /*!  cannot be changed after sending. */
  auto *board = static_cast<Board *>(layout()->itemAt(0)->widget());
  if (board == nullptr) board = static_cast<Board *>(layout()->itemAt(1)->widget());
  if (board == nullptr) return; /*!< C'est impossible... */
  if (_t == Theme::White)       board->setStyleSheet(board->white_style);
  else if (_t == Theme::Black)  board->setStyleSheet(board->black_style);
  else if (_t == Theme::Red)    board->setStyleSheet(board->red_style);
  else if (_t == Theme::Green)  board->setStyleSheet(board->green_style);
  else if (_t == Theme::Blue)   board->setStyleSheet(board->blue_style);
  else if (_t == Theme::Yellow) board->setStyleSheet(board->yellow_style);
}

/*! @brief Customizes layout of message from Jeff. */
void Message::setup_jeff() {
  QPair<QSpacerItem *, Board *> ws = make_layout();
  layout()->addWidget(ws.second);
  layout()->addItem(ws.first);
}

/*! @brief Customizes layout of message from user. */
void Message::setup_user() {
  QPair<QSpacerItem *, Board *> ws = make_layout();
  layout()->addItem(ws.first);
  layout()->addWidget(ws.second);
}

/*! @brief Displays plain text. */
void Message::setup_text(const QString &content) {
  auto *label = new QLabel(content, this);
  w = label;
  label->setObjectName("text");
  label->setTextFormat(Qt::PlainText);
  label->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
  label->setFocusPolicy(Qt::NoFocus);
  grid_layout->addWidget(w);
}

/*! @brief Displays markdown text. */
void Message::setup_markdown(const QString &content) {
  setup_text(content);
  auto *label = static_cast<QLabel *>(w);
  label->setTextFormat(Qt::RichText);
  label->setText(label->text().replace("\n", "<br>"));
}

/*! @brief Displays a picture with the path @a content. */
void Message::setup_picture(const QString &content) {
  auto *label = new QLabel(content, this);
  w = label;
  auto pix = QPixmap(content);
  if (pix.isNull()) return;
  label->setScaledContents(true);
  label->setPixmap(pix.scaledToWidth(320, Qt::SmoothTransformation));
  label->setContextMenuPolicy(Qt::CustomContextMenu);
  auto *copy_path_action = new QAction(
    QIcon::fromTheme("edit-copy", QIcon(":/arts/icons/16/copy.svg")), tr("Copy image path"), this
  );
  connect(copy_path_action, &QAction::triggered, this, [this, content] {
    auto *clipboard = QGuiApplication::clipboard();
    clipboard->setText(content);
  });
  auto *context_menu = new Menu(this);
  context_menu->addAction(copy_path_action);
  connect(label, &QLabel::customContextMenuRequested, this, [this, context_menu] {
    context_menu->exec(QCursor::pos());
  });
  grid_layout->addWidget(w);
}

/*! @brief Displays a file with the path @a content. */
// void Message::setup_file(QString path) {}

/*! @brief Displays a warning @a content. */
void Message::setup_warning(const QString &content) {
  auto *board = static_cast<Board *>(layout()->itemAt(0)->widget());
  board->setStyleSheet(board->warning_style);
  setup_text(QString(tr("Warning: ") + content));
}

/*! @brief Displays an error @a content. */
void Message::setup_error(const QString &content) {
  auto *board = static_cast<Board *>(layout()->itemAt(0)->widget());
  board->setStyleSheet(board->error_style);
  setup_text(QString(tr("Error: ") + content));
}

/*! @brief Prepares Message for widget installation. */
void Message::prepare_to_widget() {
  grid_layout->parentWidget()->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
}

/*! @brief Creates a spacer and an Board to adjust the layout. */
QPair<QSpacerItem *, Board *> Message::make_layout() {
  auto *spacer = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
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
  if (md.content_type == ContentType::Markdown) textDocument.setHtml(label->text());
  else if (
    md.content_type == ContentType::Widget or 
    md.content_type == ContentType::File or
    md.content_type == ContentType::Picture
  ) return;
  else textDocument.setPlainText(label->text());
  if (textDocument.idealWidth() < width)
    label->setWordWrap(false);
  else {
    label->setWordWrap(true);
    label->setFixedWidth(width);
  }
  _width = width;
}
