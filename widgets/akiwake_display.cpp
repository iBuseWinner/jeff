#include "akiwake_display.h"

AkiwakeDisplay::AkiwakeDisplay(QWidget *parent) : QScrollArea(parent) {
  // Creates a zone for displaying messages.
  this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  this->setFocusPolicy(Qt::NoFocus);
  this->setFrameStyle(QFrame::NoFrame);
  this->setFrameShadow(QFrame::Plain);
  this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  this->setWidgetResizable(true);
  this->setObjectName("display");
  this->layout = new QVBoxLayout(this);
  this->layout->setSpacing(0);
  this->layout->setMargin(0);
  auto *lineSpacer =
      new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Expanding);
  QWidget *box = new QWidget(this);
  box->setObjectName("box");
  this->setStyleSheet(
      "#display, #box { background-color: rgba(255, 255, 255, 0); }");
  this->layout->addItem(lineSpacer);
  box->setLayout(this->layout);
  this->setWidget(box);
  this->connector();
}

void AkiwakeDisplay::connector() {
  // Connects signals with slots.
  connect(this->verticalScrollBar(), &QScrollBar::rangeChanged, this,
          &AkiwakeDisplay::scrollDown);
  connect(this->verticalScrollBar(), &QScrollBar::valueChanged, this,
          &AkiwakeDisplay::scrollTumbler);
}

void AkiwakeDisplay::scrollTumbler(int value) {
  // Turns off and on autoscroll, reacting to user scrolling.
  if (value != this->verticalScrollBar()->maximum())
    this->scrollEnabled = false;
  else
    this->scrollEnabled = true;
}

void AkiwakeDisplay::scrollDown(int min, int max) {
  // Scrolls the display down.
  Q_UNUSED(min)
  if (this->scrollEnabled)
    this->verticalScrollBar()->setValue(max);
}
