#include "line.h"

/*! @brief The constructor. */
Line::Line(QWidget *parent) : Board(parent) {
  send_button.setText("");
  send_button.setIcon(QIcon::fromTheme("mail-send", QIcon(":/arts/icons/16/send.svg")));
  line_edit.hide_border();
  line_edit.setPlaceholderText(tr("Enter your message..."));
  setLayout(HLineLt::another()->spacing(3)->addw(&line_edit)->addw(&send_button));
  connect(&line_edit, &TextEdit::enter_triggered, &send_button, &Button::click);
}
