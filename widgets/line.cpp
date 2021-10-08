#include "line.h"

/*!
 * @fn Line::Line
 * @brief The constructor.
 * @param[in,out] parent QObject parent
 */
Line::Line(QWidget *parent) : Board(parent) {
  lineEdit = new LineEdit(this);
  sendButton = new Button("", this);
  sendButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
#ifdef Q_OS_UNIX
  sendButton->setIcon(QIcon::fromTheme("mail-send"));
#else
  sendButton->setIcon(QIcon(":/arts/icons/16/send.svg"));
#endif
  auto *layout = new QHBoxLayout(this);
  layout->setMargin(0);
  layout->setSpacing(0);
  layout->addWidget(lineEdit);
  layout->addWidget(sendButton);
  setLayout(layout);
}
