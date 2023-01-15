#include "line.h"

/*! @brief The constructor. */
Line::Line(QWidget *parent) : Board(parent) {
  send_button.setText("");
  send_button.setIcon(QIcon::fromTheme("mail-send", QIcon(":/arts/icons/16/send.svg")));
  line_edit.hide_border();
  setLayout(HLineLt::another()->spacing(3)->addw(&line_edit)->addw(&send_button));
}
