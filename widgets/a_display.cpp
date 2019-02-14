#include "a_display.h"

ADisplay::ADisplay(QWidget *parent) : QScrollArea(parent) {
  // Creates a zone for displaying messages.
  this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  this->setFocusPolicy(Qt::NoFocus);
  this->setFrameStyle(QFrame::NoFrame);
  this->setFrameShadow(QFrame::Plain);
  this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
  this->setWidgetResizable(true);
  this->setObjectName("display");
  this->start();
  this->connector();
}

void ADisplay::connector() {
  // Connects signals with slots.
  connect(this->verticalScrollBar(), &QScrollBar::rangeChanged, this,
          &ADisplay::scrollDown);
  connect(this->verticalScrollBar(), &QScrollBar::valueChanged, this,
          &ADisplay::scrollTumbler);
}

void ADisplay::start() {
  if (this->layout != nullptr)
    delete this->layout;
  QWidget *box = new QWidget(this);
  box->setObjectName("box");
  this->setStyleSheet(
      "#display, #box { background-color: rgba(255, 255, 255, 0); }");
  auto *lineSpacer =
      new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Expanding);
  this->layout = new QVBoxLayout(this);
  this->layout->setSpacing(0);
  this->layout->setMargin(0);
  this->layout->addItem(lineSpacer);
  box->setLayout(this->layout);
  this->setWidget(box);
}

void ADisplay::scrollTumbler(int value) {
  // Turns off and on autoscroll, reacting to user scrolling.
  if (value != this->verticalScrollBar()->maximum())
    this->scrollEnabled = false;
  else
    this->scrollEnabled = true;
}

void ADisplay::scrollDown(int min, int max) {
  // Scrolls the display down.
  Q_UNUSED(min)
  if (this->scrollEnabled)
    this->verticalScrollBar()->setValue(max);
}
