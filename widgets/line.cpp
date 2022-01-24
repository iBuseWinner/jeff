#include "line.h"

/*!
 * @fn Line::Line
 * @brief The constructor.
 * @param[in,out] parent QObject parent
 */
Line::Line(QWidget *parent) : Board(parent) {
  send_button.setText("");
  send_button.setToolButtonStyle(Qt::ToolButtonIconOnly);
#ifdef Q_OS_UNIX
  send_button.setIcon(QIcon::fromTheme("mail-send"));
#else
  send_button.setIcon(QIcon(":/arts/icons/16/send.svg"));
#endif
  layout.setMargin(0);
  layout.setSpacing(0);
  layout.addWidget(&line_edit);
  layout.addWidget(&send_button);
  setLayout(&layout);
}
