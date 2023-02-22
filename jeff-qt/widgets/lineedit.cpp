#include "lineedit.hpp"

/*! @brief The constructor. */
LineEdit::LineEdit(QWidget *parent) : QLineEdit(parent) {
  setStyleSheet(
    style_sheet.arg(styling.css_bg_color).arg(styling.css_fg_color).arg(styling.css_hl_color)
  );
  setPlaceholderText(tr("Enter your message..."));
  setFrame(false);
  setup_context_menu();
  setContextMenuPolicy(Qt::CustomContextMenu);
  connect(this, &LineEdit::customContextMenuRequested, this, [this] {
    context_menu->exec(QCursor::pos());
  });
}

/*! @brief Sets up context menu for use. */
void LineEdit::setup_context_menu() {
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
  connect(delete_text_action, &QAction::triggered, this, &LineEdit::backspace);
  connect(cut_text_action, &QAction::triggered, this, &LineEdit::cut);
  connect(copy_text_action, &QAction::triggered, this, &LineEdit::copy);
  connect(paste_text_action, &QAction::triggered, this, &LineEdit::paste);
  connect(select_all_text_action, &QAction::triggered, this, &LineEdit::selectAll);
  context_menu = new Menu(this);
  context_menu->addAction(delete_text_action);
  context_menu->addAction(cut_text_action);
  context_menu->addAction(copy_text_action);
  context_menu->addAction(paste_text_action);
  context_menu->addSeparator();
  context_menu->addAction(select_all_text_action);
}

/*! @brief Sets a different widget style sheet so that its border is not displayed on the screen. */
void LineEdit::hide_border() {
  setStyleSheet(
    wob_style_sheet.arg(styling.css_bg_color).arg(styling.css_fg_color)
  );
}
