#include "a_display.h"

ADisplay::ADisplay(QWidget *p) : QScrollArea(p) {
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
  setFocusPolicy(Qt::NoFocus);
  setFrameStyle(QFrame::NoFrame);
  setFrameShadow(QFrame::Plain);
  setWidgetResizable(true);
  setObjectName("display");
  start();
  connector();
}

void ADisplay::start() {
  delete l;
  QWidget *b = new QWidget(this);
  b->setObjectName("box");
  setStyleSheet("#display, #box { background-color: rgba(255, 255, 255, 0); }");
  auto *ls = new QSpacerItem(0, 0, QSizePolicy::Fixed, QSizePolicy::Expanding);
  l = new QVBoxLayout(this);
  l->setSpacing(0);
  l->setMargin(0);
  l->addItem(ls);
  b->setLayout(l);
  setWidget(b);
}

void ADisplay::connector() {
  connect(verticalScrollBar(), &QScrollBar::rangeChanged, this,
          &ADisplay::scrollDown);
  connect(verticalScrollBar(), &QScrollBar::valueChanged, this,
          &ADisplay::scrollTumbler);
}

void ADisplay::scrollDown(int min, int max) {
  Q_UNUSED(min)
  if (se) verticalScrollBar()->setValue(max);
}


void ADisplay::scrollTumbler(int v) {
  if (v != verticalScrollBar()->maximum())
    se = false;
  else
    se = true;
}
