#include "line.h"

/*!
 * @fn Line::Line
 * @brief The constructor.
 * @param[in,out] parent QObject parent
 */
Line::Line(QWidget *parent) : Board(parent) {
  send_button.setText("");
  send_button.setToolButtonStyle(Qt::ToolButtonIconOnly);
  send_button.setIcon(QIcon::fromTheme("mail-send", QIcon(":/arts/icons/16/send.svg")));
  layout.setMargin(0);
  layout.setSpacing(0);
  layout.addWidget(&line_edit);
  layout.addWidget(&send_button);
  setLayout(&layout);
}
