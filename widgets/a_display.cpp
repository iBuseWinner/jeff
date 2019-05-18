#include "a_display.h"

/*
 * All short named objects and their explanations:
 * {sp} <- spacer
 * {lt} <- layout
 * {v} <- value
 * {min} <- minimum
 * {max} <- maximum
 */

/*!
 * Argument: QWidget {*parent}.
 * Creates an ADisplay.
 */
ADisplay::ADisplay(QWidget *parent) : QScrollArea(parent) {
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

/*! Sets the widget to its initial state. */
void ADisplay::start() {
  if (lt == nullptr) delete lt;
  QWidget *box = new QWidget(this);
  box->setObjectName("box");
  box->setAttribute(Qt::WA_DeleteOnClose);
  setStyleSheet("#display, #box { background-color: rgba(255, 255, 255, 0); }");
  auto *sp = new QSpacerItem(0, 0, QSizePolicy::Fixed, QSizePolicy::Expanding);
  lt = new QVBoxLayout(this);
  lt->setSpacing(0);
  lt->setMargin(0);
  lt->addItem(sp);
  box->setLayout(lt);
  setWidget(box);
}

/*! Establishes communications for user interaction through the widget. */
void ADisplay::connector() {
  connect(verticalScrollBar(), &QScrollBar::rangeChanged, this,
          &ADisplay::scrollDown);
  connect(verticalScrollBar(), &QScrollBar::valueChanged, this,
          &ADisplay::scrollTumbler);
}

/*!
 * Arguments: int {min} [transmitted automatically by Qt],
 *            int {max} [transmitted automatically by Qt].
 * When the scrolling range of the display changes, it scrolls to the end if
 * automatic scrolling is enabled.
 */
void ADisplay::scrollDown(int min, int max) {
  Q_UNUSED(min)
  if (scrollEnabled) verticalScrollBar()->setValue(max);
}

/*!
 * Argument: int {v} [current position of the vertical scroll bar].
 * Enables or disables automatic scrolling.
 */
void ADisplay::scrollTumbler(int v) {
  if ((!scrollEnabled) && (v == verticalScrollBar()->maximum()))
    scrollEnabled = true;
  if ((scrollEnabled) && (v != verticalScrollBar()->maximum()))
    scrollEnabled = false;
}
