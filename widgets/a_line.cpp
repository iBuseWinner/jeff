#include "a_line.h"

/*
 * All short named objects and their explanations:
 * {lt} <- layout
 */

/*!
 * Argument: QWidget {*parent}.
 * Creates an ALine.
 */
ALine::ALine(QWidget *parent) : ABoard(parent) {
  lineEdit = new ALineEdit(this);
  sendButton = new AButton("", this);
  sendButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
  sendButton->setIcon(QIcon(":/arts/icons/16/send.svg"));
  auto *lt = new QHBoxLayout(this);
  lt->setMargin(0);
  lt->setSpacing(0);
  lt->addWidget(lineEdit);
  lt->addWidget(sendButton);
  setLayout(lt);
}
