#include "textedit.hpp"

/*! @brief The constructor. */
TextEdit::TextEdit(QWidget *parent) : QTextEdit(parent) {
  clear(text_height());
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
  connect(this, &TextEdit::textChanged, this, &TextEdit::resize_on_text_height);
  setStyleSheet(
    style_sheet.arg(styling.css_bg_color).arg(styling.css_fg_color).arg(styling.css_hl_color)
  );
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  verticalScrollBar()->setStyleSheet(
    styling.css_scroll_bar.arg(styling.light_theme ? styling.css_light_sb : styling.css_dark_sb)
  );
  setFrameStyle(QFrame::NoFrame);
  setFrameShadow(QFrame::Plain);
  verticalScrollBar()->setFixedWidth(5);
  setTabChangesFocus(true);
  setTabStopDistance(40);
  setup_context_menu();
  setContextMenuPolicy(Qt::CustomContextMenu);
  connect(this, &TextEdit::customContextMenuRequested, this, [this]() {
    context_menu->exec(QCursor::pos());
  });
}

/*! @brief Compat function for @a QLineEdit::backspace. */
void TextEdit::backspace() { textCursor().deletePreviousChar(); }

/*! @brief Compat function for @a QLineEdit::insert. */
void TextEdit::insert(const QString &text) { textCursor().insertText(text); }

/*! @brief Compat function for @a QLineEdit::text. */
QString TextEdit::text() { return toPlainText(); }

/*! @brief Sets up context menu for use. */
void TextEdit::setup_context_menu() {
  delete_text_action = new QAction(this);
  cut_text_action = new QAction(this);
  copy_text_action = new QAction(this);
  paste_text_action = new QAction(this);
  select_all_text_action = new QAction(this);
  delete_text_action->setText(tr("Delete"));
  cut_text_action->setText(tr("Cut"));
  copy_text_action->setText(tr("Copy"));
  paste_text_action->setText(tr("Paste"));
  select_all_text_action->setText(tr("Select all"));
  delete_text_action->setIcon(
    QIcon::fromTheme("edit-clear", QIcon(":/arts/icons/16/clear.svg")));
  cut_text_action->setIcon(
    QIcon::fromTheme("edit-cut", QIcon(":/arts/icons/16/cut.svg")));
  copy_text_action->setIcon(
    QIcon::fromTheme("edit-copy", QIcon(":/arts/icons/16/copy.svg")));
  paste_text_action->setIcon(
    QIcon::fromTheme("edit-paste", QIcon(":/arts/icons/16/paste.svg")));
  select_all_text_action->setIcon(
    QIcon::fromTheme("edit-select-all", QIcon(":/arts/icons/16/select-all.svg")));
  connect(delete_text_action, &QAction::triggered, this, &TextEdit::backspace);
  connect(cut_text_action, &QAction::triggered, this, &TextEdit::cut);
  connect(copy_text_action, &QAction::triggered, this, &TextEdit::copy);
  connect(paste_text_action, &QAction::triggered, this, &TextEdit::paste);
  connect(select_all_text_action, &QAction::triggered, this, &TextEdit::selectAll);
  context_menu = new Menu(this);
  context_menu->addAction(delete_text_action);
  context_menu->addAction(cut_text_action);
  context_menu->addAction(copy_text_action);
  context_menu->addAction(paste_text_action);
  context_menu->addSeparator();
  context_menu->addAction(select_all_text_action);
}

/*! @brief Sets a different widget style sheet so that its border is not displayed on the screen. */
void TextEdit::hide_border() {
  setStyleSheet(
    wob_style_sheet.arg(styling.css_bg_color).arg(styling.css_fg_color)
  );
}

/*! @brief Sets TextEdit' height from content height. */
void TextEdit::resize_on_text_height() {
  auto height = text_height();
  if (curr_height == height) return;
  else if (curr_height > height) {
    if (line_cntr == 1) return;
    else line_cntr--;
  } else if (curr_height < height) {
    if (line_cntr == 4) return;
    else line_cntr++;
  }
  curr_height = height;
  setFixedHeight(curr_height);
}

/*! @brief Clears the content of the input field.
 *  @details When created, the widget requires the widget's height to be forced before the content is cleared,
 *  otherwise the height will be 0 pixels.  */
void TextEdit::clear(int presize) {
  QTextEdit::clear();
  curr_height = (presize == -1) ? text_height() : presize;
  setFixedHeight(curr_height);
  line_cntr = 1;
}

/*! @brief Calculates the height of the text in pixels. */
int TextEdit::text_height() {
  auto size = document()->size().toSize();
  return size.height();
}

/*! @brief Handles keyboard shortcuts. */
void TextEdit::keyPressEvent(QKeyEvent *event) {
  if ((event->modifiers() == Qt::ShiftModifier) and (event->key() == Qt::Key_Return)) insert("\n");
  else if (event->key() == Qt::Key_Return) emit enter_triggered();
  else QTextEdit::keyPressEvent(event);
}
