#include "a_line.h"

/*!
 * @fn ALine::ALine
 * @brief The constructor.
 * @param[in,out] parent QObject parent
 */
ALine::ALine(QWidget *parent) : ABoard(parent) {
  lineEdit = new ALineEdit(this);
  sendButton = new AButton("", this);
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
