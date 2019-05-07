#include "a_line.h"

ALine::ALine(QWidget *p) : ABoard(p) {
  tl = new ALineEdit(this);
  s = new AButton("", this);
  s->setToolButtonStyle(Qt::ToolButtonIconOnly);
  s->setIcon(QIcon(":/arts/icons/16/send.svg"));
  connect(tl, &QLineEdit::returnPressed, s, &AButton::click);
  auto *el = new QHBoxLayout(this);
  el->setMargin(0);
  el->setSpacing(0);
  el->addWidget(tl);
  el->addWidget(s);
  setLayout(el);
}
