#include "a_line.h"

ALine::ALine(QWidget *p) : ABoard(p) {
  // Creates a user input widget.
  tl = new ALineEdit(this);
  s = new APushButton("Send", this);
  s->setIcon(QIcon(":/arts/icons/16/send.svg"));
  s->setFocusPolicy(Qt::NoFocus);
  connect(tl, &QLineEdit::returnPressed, s, &APushButton::click);
  auto *el = new QHBoxLayout(this);
  el->setMargin(0);
  el->setSpacing(0);
  el->addWidget(tl);
  el->addWidget(s);
  setLayout(el);
}
