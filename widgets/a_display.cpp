#include "a_display.h"
#ifdef ADISPLAY_DEBUG
#include <QDebug>
#include <QElapsedTimer>
#endif

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
void ADisplay::addMessage(AMessage *message) {
#ifdef ADISPLAY_ADDMSG_DEBUG
  QElapsedTimer timer;
  timer.start();
#endif
  messages_mutex.lock();
  message_counter++;
  message->setParent(this);
  connect(message, &AMessage::closed, this,
          [this, message] { removeMessage(message); });
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
      all_messages[all_messages.length() - message_counter]->hide();
      vertical_box_layout->removeWidget(
          all_messages[all_messages.length() - message_counter--]);
    }
  messages_mutex.unlock();
#ifdef ADISPLAY_ADDMSG_DEBUG
  qDebug() << "ADisplay::addMessage:" << timer.nsecsElapsed();
#endif
}

/*! Sets the widget to its initial state. */
void ADisplay::start() {
#ifdef ADISPLAY_START_DEBUG
  qDebug() << "ADisplay::start: we are here.";
#endif
  messages_mutex.lock();
#ifdef ADISPLAY_START_DEBUG
  qDebug() << "ADisplay::start: mutex locked.";
#endif
  all_messages.clear();
#ifdef ADISPLAY_START_DEBUG
  qDebug() << "ADisplay::start: widgets cleared.";
#endif
  if (vertical_box_layout)
    delete vertical_box_layout;
#ifdef ADISPLAY_START_DEBUG
  qDebug() << "ADisplay::start: vertical_box_layout deleted.";
#endif
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
  messages_mutex.unlock();
#ifdef ADISPLAY_START_DEBUG
  qDebug() << "ADisplay::start: mutex unlocked.";
#endif
#ifdef ADISPLAY_START_DEBUG
  qDebug() << "ADisplay::start: we are ready.";
#endif
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
 * Argument: int {value} [current position of the vertical scroll bar].
 * Enables or disables automatic scrolling.
 */
void ADisplay::scrollTumbler(int value) {
  if (not scrollEnabled and (value == verticalScrollBar()->maximum()))
    scrollEnabled = true;
  if (scrollEnabled and (value != verticalScrollBar()->maximum()))
    scrollEnabled = false;
}

void ADisplay::showWidgets(int value) {
  if ((value == verticalScrollBar()->minimum()) and
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

void ADisplay::removeMessage(AMessage *message) {
  messages_mutex.lock();
  message->close();
  vertical_box_layout->removeWidget(message);
#ifdef ADISPLAY_DEBUG
  if (not
#endif
      all_messages.removeOne(message)
#ifdef ADISPLAY_DEBUG
  )
    qDebug() << "message has not been removed from list!";
#else
      ;
#endif
  disconnect(message);
  message_counter--;
  messages_mutex.unlock();
}
