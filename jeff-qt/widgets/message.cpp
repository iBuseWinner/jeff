#include "message.hpp"

/*! @brief The constructor. */
Message::Message() { setLayout(HLineLt::another()->cmargins(0, 0, 0, standardMargin)->spacing(0)); }
/*! @brief Creates a Message based on @a message. */
Message::Message(MessageMeta *_md) : Message() { message_data(_md); }

/*! @brief Sets @a _message_data into the Message. */
void Message::message_data(MessageMeta *_md) {
  if (not _md) return;
  md = _md;
  author();
  content_type();
  theme();
}

/*! @brief Sets {modal_handler->getPrisoner()} into the Message. */
void Message::widget(ModalHandler *modal_handler) {
  if (w) return;
  w = modal_handler->getPrisoner();
  connect(modal_handler, &ModalHandler::prisonerClosed, this, [this] { emit closed(); });
  grid_layout->addw(w);
}

/*! @brief Makes a message either left or right. */
void Message::author() {
  if (md->author == Author::Jeff) setup_jeff();
  else setup_user();
}

/*! @brief Updates the text of a message. */
void Message::update_text(const QString &text) {
  md->content = text;
  if (md->content_type == ContentType::Markdown) precached_md = from_plain_to_markdown(md->content);
  fit_text(_width);
}

/*! @brief Adjusts the message to the content type. */
void Message::content_type() {
  if (md->content_type == ContentType::Picture or
      md->content.toLower().endsWith(".jpg") or
      md->content.toLower().endsWith(".png")) {
    auto info = QFileInfo(md->content);
    if (info.exists() and info.isFile()) {
      md->content_type = ContentType::Picture;
      setup_picture();
    } else setup_text();
  }
  else if (md->content_type == ContentType::Text) setup_text();
  else if (md->content_type == ContentType::Markdown) setup_markdown();
  // else if (md->content_type == ContentType::File) setup_file();
  else if (md->content_type == ContentType::Warning) setup_warning();
  else if (md->content_type == ContentType::Error) setup_error();
  else prepare_to_widget();
}

/*! @brief Sets the message colors. */
void Message::theme() {
  if (md->theme == Theme::Std) return; /*!< This theme is by default, and message theme
                                       *   cannot be changed after sending. */
  auto *board = static_cast<Board *>(layout()->itemAt(0)->widget());
  if (board == nullptr) board = static_cast<Board *>(layout()->itemAt(1)->widget());
  if (board == nullptr) return; /*!< C'est impossible... */
  if (md->theme == Theme::White)       board->setStyleSheet(board->white_style);
  else if (md->theme == Theme::Black)  board->setStyleSheet(board->black_style);
  else if (md->theme == Theme::Red)    board->setStyleSheet(board->red_style);
  else if (md->theme == Theme::Green)  board->setStyleSheet(board->green_style);
  else if (md->theme == Theme::Blue)   board->setStyleSheet(board->blue_style);
  else if (md->theme == Theme::Yellow) board->setStyleSheet(board->yellow_style);
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
void Message::setup_text() {
  auto *label = new QLabel(md->content, this);
  w = label;
  label->setObjectName("text");
  label->setTextFormat(Qt::PlainText);
  label->setTextInteractionFlags(Qt::LinksAccessibleByMouse | Qt::TextSelectableByMouse);
  label->setContextMenuPolicy(Qt::NoContextMenu);
  label->setFocusPolicy(Qt::NoFocus);
  auto *copy_text_action = new QAction(
    QIcon::fromTheme("edit-copy", QIcon(":/arts/icons/16/copy.svg")), tr("Copy message text"), this
  );
  connect(copy_text_action, &QAction::triggered, this, [this, label] {
    auto *clipboard = QGuiApplication::clipboard();
    if (not label->selectedText().isEmpty())
      clipboard->setText(label->selectedText());
    else
      clipboard->setText(this->message_data()->content);
  });
  auto *context_menu = new Menu(this);
  context_menu->addAction(copy_text_action);
  setContextMenuPolicy(Qt::CustomContextMenu);
  connect(this, &Message::customContextMenuRequested, this, [this, label, copy_text_action, context_menu] {
    if (not label->selectedText().isEmpty())
      copy_text_action->setText(tr("Copy selected text"));
    else
      copy_text_action->setText(tr("Copy message text"));
    context_menu->exec(QCursor::pos());
  });
  grid_layout->addw(w);
}

/*! @brief Displays markdown text. */
void Message::setup_markdown() {
  setup_text();
  auto *label = static_cast<QLabel *>(w);
  label->setTextFormat(Qt::RichText);
  precached_md = from_plain_to_markdown(md->content);
  label->setText(precached_md);
}

/*! @brief Turns a plain text with Markdown syntax into HTML. */
QString Message::from_plain_to_markdown(QString content) {
  auto preformatted = content.replace("\t", "&nbsp;&nbsp;&nbsp;&nbsp;").replace("_", "&#95;");
  auto markdown = preformatted.toStdString();
  hoedown_buffer *buf = hoedown_buffer_new(16);
  hoedown_renderer *renderer = hoedown_html_renderer_new(HOEDOWN_HTML_ESCAPE, 0);
  hoedown_document *document = hoedown_document_new(renderer, HOEDOWN_EXT_TABLES, 16);
  hoedown_document_render(document, buf, (uint8_t*)markdown.c_str(), markdown.size());
  auto html = std::string((char*)buf->data, buf->size);
  hoedown_buffer_free(buf);
  hoedown_document_free(document);
  hoedown_html_renderer_free(renderer);
  auto formatted = QString::fromStdString(html);
  return formatted;
}

/*! @brief Displays a picture with the path @a content. */
void Message::setup_picture() {
  auto *label = new QLabel(md->content, this);
  w = label;
  auto pix = QPixmap(md->content);
  if (pix.isNull()) return;
  label->setScaledContents(true);
  label->setPixmap(pix.scaledToWidth(640, Qt::SmoothTransformation));
  if (label->pixmap(Qt::ReturnByValue).height() > 480) label->setPixmap(pix.scaledToHeight(480, Qt::SmoothTransformation));
  label->setContextMenuPolicy(Qt::CustomContextMenu);
  auto *open_action = new QAction(
    QIcon::fromTheme("view-preview", QIcon(":/arts/icons/16/view-preview.svg")),
    tr("Open"), this
  );
  auto *copy_path_action = new QAction(
    QIcon::fromTheme("edit-copy-path", QIcon(":/arts/icons/16/edit-copy-path.svg")),
    tr("Copy image path"), this
  );
  connect(open_action, &QAction::triggered, this, [this] {
    QDesktopServices::openUrl(QUrl::fromLocalFile(this->message_data()->content));
  });
  connect(copy_path_action, &QAction::triggered, this, [this] {
    auto *clipboard = QGuiApplication::clipboard();
    clipboard->setText(this->message_data()->content);
  });
  auto *context_menu = new Menu(this);
  context_menu->addAction(open_action);
  context_menu->addAction(copy_path_action);
  connect(label, &QLabel::customContextMenuRequested, this, [this, context_menu] {
    context_menu->exec(QCursor::pos());
  });
  grid_layout->addw(w);
}

/*! @brief Displays a file with the path @a content. */
// void Message::setup_file() {}

/*! @brief Displays a warning @a content. */
void Message::setup_warning() {
  auto *board = static_cast<Board *>(layout()->itemAt(0)->widget());
  board->setStyleSheet(board->warning_style);
  setup_text();
  auto *label = static_cast<QLabel *>(w);
  label->setTextFormat(Qt::RichText);
  label->setText(from_plain_to_markdown(QString(tr("Warning: ") + md->content)));
}

/*! @brief Displays an error @a content. */
void Message::setup_error() {
  auto *board = static_cast<Board *>(layout()->itemAt(0)->widget());
  board->setStyleSheet(board->error_style);
  setup_text();
  auto *label = static_cast<QLabel *>(w);
  label->setTextFormat(Qt::RichText);
  label->setText(from_plain_to_markdown(QString(tr("Error: ") + md->content)));
}

/*! @brief Prepares Message for widget installation. */
void Message::prepare_to_widget() {
  grid_layout->parentWidget()->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

/*! @brief Creates a spacer and an Board to adjust the layout. */
QPair<QSpacerItem *, Board *> Message::make_layout() {
  auto *spacer = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
  auto *board = new Board(this);
  grid_layout = GridLt::another()->spacing(0)->margin(standardMargin);
  board->setLayout(grid_layout);
  return QPair<QSpacerItem *, Board *>(spacer, board);
}

/*! @brief Fits the text to the size of the message. */
void Message::setWidth(int width) {
  if (_width == width) return;
  fit_text(width);
  _width = width;
}

/*! @brief TBD */
void Message::fit_text(int width) {
  auto *label = qobject_cast<QLabel *>(w);
  if (not label) {
    if (w) w->setMaximumWidth(width);
    return;
  }
  if (
    md->content_type == ContentType::Widget or
    md->content_type == ContentType::File or
    md->content_type == ContentType::Picture
  ) return;
  QString content;
  if (md->content_type == ContentType::Markdown) content = precached_md;
  else content = md->content;
  QString rtext, next_line;
  int i = 0;
  while (not (next_line = optimal_line(content, i, 80)).isEmpty()) {
    if (not rtext.isEmpty()) {
      if (md->content_type == ContentType::Markdown) rtext.append("<br>");
      else if (
        md->content_type == ContentType::Text or
        md->content_type == ContentType::Warning or
        md->content_type == ContentType::Error
      ) rtext.append("\n");
    }
    rtext.append(next_line);
    i += next_line.length();
  }
  label->setText(rtext);
}

/*! @brief Returns @a line that fits @a max_sym_width. */
QString Message::optimal_line(const QString &remaining, const int &start_index, int max_sym_width) {
  QString line, word;
  int cntr = 0, sc = 0;
  for (const QChar &c : remaining) {
    if (sc++ < start_index) continue;
    if (cntr++ >= 80) break;
    word += c;
    if (c.isSpace()) {
      line.append(word);
      word.clear();
    }
  }
  /*! Splitting by symbols of very long word. */
  if (line.isEmpty() and not word.isEmpty()) {
    QChar next_sym;
    for (int i = 0; (i < word.length()) and (line.length() + 1 <= max_sym_width); i++) {
      if (not next_sym.isNull()) line.append(next_sym);
      next_sym = word.at(i);
    }
  }
  return line;
}
