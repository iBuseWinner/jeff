#include "a_display.h"

ADisplay::ADisplay(QWidget *p) : QScrollArea(p) {
  // Creates a zone for displaying messages.
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

void ADisplay::connector() {
  // Connects signals with slots.
  connect(verticalScrollBar(), &QScrollBar::rangeChanged, this,
          &ADisplay::scrollDown);
  connect(verticalScrollBar(), &QScrollBar::valueChanged, this,
          &ADisplay::scrollTumbler);
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

void ADisplay::scrollTumbler(int v) {
  // Turns off and on autoscroll, reacting to user scrolling.
  if (v != verticalScrollBar()->maximum())
    se = false;
  else
    se = true;
}

void ADisplay::scrollDown(int min, int max) {
  // Scrolls the display down.
  Q_UNUSED(min)
  if (se) verticalScrollBar()->setValue(max);
}
