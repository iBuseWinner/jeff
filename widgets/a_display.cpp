#include "a_display.h"
#include <QElapsedTimer>
#include <QDebug>

/*
 * All short named objects and their explanations:
 * {sp} <- spacer
 * {v} <- value
 * {min} <- minimum
 * {max} <- maximum
 */

/*!
 * Argument: QWidget {*parent}.
 * Creates an ADisplay.
 */
ADisplay::ADisplay(short _max_message_amount, QWidget *parent)
    : QScrollArea(parent), max_message_amount(_max_message_amount) {
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

/*! Adds a message to the display. */
void ADisplay::addMessage(QWidget *message) {
  QElapsedTimer timer;
  timer.start();
  message_counter++;
  message->setParent(this);
  all_messages.append(message);
  vertical_box_layout->addWidget(message);
  // если скролл примерно ниже середины, и число отображаемых сообщений больше
  // максимума
  if ((verticalScrollBar()->value() >
       ((verticalScrollBar()->minimum() + verticalScrollBar()->maximum()) /
        2)) and
      (message_counter > max_message_amount))
    // удаляем все лишние сообщения
    while (message_counter > max_message_amount) {
      all_messages.at(all_messages.length() - message_counter)->hide();
      vertical_box_layout->removeWidget(
          all_messages.at(all_messages.length() - message_counter--));
    }
  qDebug() << "ADisplay::addMessage:" << timer.nsecsElapsed();
}

/*! Sets the widget to its initial state. */
void ADisplay::start() {
  for (QWidget *widget : qAsConst(all_messages))
    widget->close();
  all_messages.clear();
  if (vertical_box_layout)
    delete vertical_box_layout;
  QWidget *box = new QWidget(this);
  box->setObjectName("box");
  box->setAttribute(Qt::WA_DeleteOnClose);
  setStyleSheet("#display, #box { background-color: rgba(255, 255, 255, 0); }");
  auto *sp = new QSpacerItem(0, 0, QSizePolicy::Fixed, QSizePolicy::Expanding);
  vertical_box_layout = new QVBoxLayout(this);
  vertical_box_layout->setSpacing(0);
  vertical_box_layout->setMargin(0);
  vertical_box_layout->addItem(sp);
  box->setLayout(vertical_box_layout);
  setWidget(box);
}

/*! Establishes communications for user interaction through the widget. */
void ADisplay::connector() {
  connect(verticalScrollBar(), &QScrollBar::rangeChanged, this,
          &ADisplay::scrollDown);
  connect(verticalScrollBar(), &QScrollBar::valueChanged, this,
          &ADisplay::scrollTumbler);
  connect(verticalScrollBar(), &QScrollBar::valueChanged, this,
          &ADisplay::showWidgets);
}

/*!
 * Arguments: int {min} [transmitted automatically by Qt],
 *            int {max} [transmitted automatically by Qt].
 * When the scrolling range of the display changes, it scrolls to the end if
 * automatic scrolling is enabled.
 */
void ADisplay::scrollDown(int min, int max) {
  Q_UNUSED(min)
  if (scrollEnabled)
    verticalScrollBar()->setValue(max);
}

/*!
 * Argument: int {v} [current position of the vertical scroll bar].
 * Enables or disables automatic scrolling.
 */
void ADisplay::scrollTumbler(int v) {
  if (not scrollEnabled and (v == verticalScrollBar()->maximum()))
    scrollEnabled = true;
  if (scrollEnabled and (v != verticalScrollBar()->maximum()))
    scrollEnabled = false;
}

void ADisplay::showWidgets(int v) {
  if ((v == verticalScrollBar()->minimum()) and
      (message_counter < all_messages.length())) {
    // добавляем по половине от максимума
    short portion = max_message_amount / 2;
    if (message_counter + portion > all_messages.length())
      portion = all_messages.length() - message_counter;
    while (portion) {
      vertical_box_layout->insertWidget(
          1, all_messages.at(all_messages.length() - message_counter - 1));
      all_messages.at(all_messages.length() - message_counter - 1)->show();
      message_counter++;
      portion--;
    }
    verticalScrollBar()->setValue(1);
  }
}
