#include "message.h"
#include <iostream>

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
  setWidth(_width);
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
  label->setTextInteractionFlags(Qt::LinksAccessibleByMouse);
  label->setFocusPolicy(Qt::NoFocus);
  label->setContextMenuPolicy(Qt::CustomContextMenu);
  auto *copy_text_action = new QAction(
    QIcon::fromTheme("edit-copy", QIcon(":/arts/icons/16/copy.svg")), tr("Copy message text"), this
  );
  connect(copy_text_action, &QAction::triggered, this, [this] {
    auto *clipboard = QGuiApplication::clipboard();
    clipboard->setText(this->content());
  });
  auto *context_menu = new Menu(this);
  context_menu->addAction(copy_text_action);
  connect(label, &QLabel::customContextMenuRequested, this, [this, context_menu] {
    context_menu->exec(QCursor::pos());
  });
  grid_layout->addWidget(w);
}

/*! @brief Displays markdown text. */
void Message::setup_markdown(const QString &content) {
  setup_text(content);
  auto *label = static_cast<QLabel *>(w);
  label->setTextFormat(Qt::RichText);
  md.content = md.content.replace("\n", "<br>");
  label->setText(md.content);
}

/*! @brief Displays a picture with the path @a content. */
void Message::setup_picture(const QString &content) {
  auto *label = new QLabel(content, this);
  w = label;
  auto pix = QPixmap(content);
  if (pix.isNull()) return;
  label->setScaledContents(true);
  label->setPixmap(pix.scaledToWidth(640, Qt::SmoothTransformation));
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
  if (
    md.content_type == ContentType::Widget or
    md.content_type == ContentType::File or
    md.content_type == ContentType::Picture
  ) return;
  if (text_ideal_width) {
    QTextDocument textDocument;
    QString rtext, next_line;
    QString content = md.content;
    while (not (next_line = optimal_line(content, textDocument, width, md.content_type)).isEmpty()) {
      if (not rtext.isEmpty()) {
        if (md.content_type == ContentType::Markdown) rtext.append("<br>");
        else if (md.content_type == ContentType::Text) rtext.append("\n");
      }
      rtext.append(next_line);
      content.remove(0, next_line.length());
    }
    label->setText(rtext);
  } else if (not non_ideal_width_completed) {
    QString rtext, next_line;
    QString content = md.content;
    while (not (next_line = optimal_line(content, 64)).isEmpty()) {
      if (not rtext.isEmpty()) {
        if (md.content_type == ContentType::Markdown) rtext.append("<br>");
        else if (md.content_type == ContentType::Text) rtext.append("\n");
      }
      rtext.append(next_line);
      content.remove(0, next_line.length());
    }
    label->setText(rtext);
    non_ideal_width_completed = true;
  }
  _width = width;
}

/*! @brief Returns @a line that fits @a max_width.
 *  @details Measures text's width for @a Message widget.
 *  @details How do we know that @a QTextDocument knows ideal width of @a text without any options?
 *  That's all 'cause we use default @a QLabel settings, setting up only text's format.  */
QString Message::optimal_line(
  const QString &remaining, QTextDocument &document, int max_width, ContentType ct
) {
  document.setPlainText("");
  auto words = remaining.split(QLatin1Char(' '));
  /*! Splitting by @a words. */
  QString line;
  QString next_part;
  while (document.idealWidth() < max_width) {
    if (not line.isEmpty()) line.append(" ");
    if (not next_part.isEmpty()) line.append(next_part);
    if (words.isEmpty()) break;
    next_part = words.takeFirst();
    if       (ct == ContentType::Markdown)  document.setMarkdown(line + " " + next_part);
    else if  (ct == ContentType::Text)      document.setPlainText(line + " " + next_part);
  }
  /*! Splitting by symbols of very long word. */
  if (line.isEmpty() and not next_part.isEmpty()) {
    document.setPlainText("");
    QChar next_sym;
    for (int i = 0; (i < next_part.length()) and (document.idealWidth() < max_width); i++) {
      if (not next_sym.isNull()) line.append(next_sym);
      next_sym = next_part.at(i);
      if       (ct == ContentType::Markdown)  document.setMarkdown(line + next_sym);
      else if  (ct == ContentType::Text)      document.setPlainText(line + next_sym);
    }
  }
  return line;
}

/*! @brief Returns @a line that fits @a max_sym_width. */
QString Message::optimal_line(const QString &remaining, int max_sym_width) {
  auto words = remaining.split(QLatin1Char(' '));
  /*! Splitting by @a words. */
  QString line;
  QString next_part;
  while (line.length() + next_part.length() < max_sym_width) {
    if (not next_part.isEmpty()) line.append(" " + next_part);
    if (words.isEmpty()) break;
    next_part = words.takeFirst();
  }
  /*! Splitting by symbols of very long word. */
  if (line.isEmpty() and not next_part.isEmpty()) {
    QChar next_sym;
    for (int i = 0; (i < next_part.length()) and (line.length() + 1 < max_sym_width); i++) {
      if (not next_sym.isNull()) line.append(next_sym);
      next_sym = next_part.at(i);
    }
  }
  return line;
}
