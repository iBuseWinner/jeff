#include "display.h"

/*!
 * @fn Display::Display
 * @brief The constructor.
 * @param[in] _max_message_amount number of messages that can be displayed
 * simultaneously
 * @param[in,out] parent QObject parent
 */
Display::Display(short _max_message_amount, QWidget *parent)
    : QScrollArea(parent), max_message_amount(_max_message_amount) {
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
  setFocusPolicy(Qt::NoFocus);
  setFrameStyle(QFrame::NoFrame);
  setFrameShadow(QFrame::Plain);
  setWidgetResizable(true);
  setObjectName(object_name);
  start();
  connector();
}

/*!
 * @fn Display::add_message
 * @brief Adds a message to the display.
 * @param[in,out] message message to be added
 */
void Display::add_message(Message *message) {
  messages_mutex.lock();
  message_counter++;
  message->setParent(this);
  connect(message, &Message::closed, this, [this, message] { removeMessage(message); });
  all_messages.append(message);
  vertical_box_layout->addWidget(message);
  /*! If the scroll is approximately below the middle, and the number of
   * displayed messages is greater than the maximum... */
  if ((verticalScrollBar()->value() >
       ((verticalScrollBar()->minimum() + verticalScrollBar()->maximum()) / 2)) and
      (message_counter > max_message_amount))
    /*! ...we delete all unnecessary messages */
    while (message_counter > max_message_amount) {
      all_messages[all_messages.length() - message_counter]->hide();
      vertical_box_layout->removeWidget(all_messages[all_messages.length() - message_counter--]);
    }
  messages_mutex.unlock();
}

/*!
 * @fn Display::add_message
 * @brief Constructs @a Message and adds it to the display.
 * @param[in] message_data message to be added
 */
void Display::add_message_by_md(MessageData message_data) {
  add_message(new Message(message_data));
}

/*!
 * @fn Display::add_message
 * @brief Constructs @a Message and adds it to the display.
 * @param[in] message_data message to be added
 * @param[in,out] handler modal handler with @a QWidget to be shown
 */
void Display::add_message_with_widget(MessageData message_data, ModalHandler *handler) {
  auto *message = new Message(message_data);
  handler->getPrisoner()->setParent(message);
//   handler->getPrisoner()->setFixedWidth(Message::maximalMessageWidth);
  message->setWidget(handler);
  add_message(message);
}

/*!
 * @fn Display::start
 * @brief Sets the widget to its initial state.
 * @details The method is also used to reset an existing state.
 */
void Display::start() {
  messages_mutex.lock();
  all_messages.clear();
  if (vertical_box_layout) delete vertical_box_layout;
  QWidget *box = new QWidget(this);
  box->setContentsMargins(0, 0, 5, 0);
  box->setObjectName(box_object_name);
  setStyleSheet(box_style_sheet);
  auto *sp = new QSpacerItem(0, 0, QSizePolicy::Fixed, QSizePolicy::Expanding);
  vertical_box_layout = new QVBoxLayout(this);
  vertical_box_layout->setSpacing(0);
  vertical_box_layout->setMargin(0);
  vertical_box_layout->addItem(sp);
  box->setLayout(vertical_box_layout);
  setWidget(box);
  messages_mutex.unlock();
  verticalScrollBar()->setStyleSheet(
      "QScrollBar:vertical { background: transparent; border: none; width: "
      "5px; margin: 0px 0px 5px 0px; } QScrollBar::handle:vertical { border: "
      "2px solid #e4e4e4; background: #e4e4e4; width: 5px; border-radius: 1px; "
      "} QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { width: "
      "0px; height: 0px; }");
}

/*!
 * @fn Display::connector
 * @brief Establishes communications for user interaction through the widget.
 */
void Display::connector() {
  connect(verticalScrollBar(), &QScrollBar::rangeChanged, this, &Display::scrollDown);
  connect(verticalScrollBar(), &QScrollBar::valueChanged, this, &Display::scrollTumbler);
  connect(verticalScrollBar(), &QScrollBar::valueChanged, this, &Display::showWidgets);
}

/*!
 * @fn Display::scrollDown
 * @brief When the scrolling range of the display changes, it scrolls to the end
 * if automatic scrolling is enabled.
 * @param[in] min minimal vertical scroll bar value
 * @param[in] max maximal vertical scroll bar value
 */
void Display::scrollDown(int min, int max) {
  Q_UNUSED(min)
  if (scrollEnabled) verticalScrollBar()->setValue(max);
}

/*!
 * @fn Display::scrollTumbler
 * @brief Enables or disables automatic scrolling.
 * @param[in] value current position of the vertical scroll bar
 */
void Display::scrollTumbler(int value) {
  if (not scrollEnabled and (value == verticalScrollBar()->maximum())) scrollEnabled = true;
  if (scrollEnabled and (value != verticalScrollBar()->maximum())) scrollEnabled = false;
}

/*!
 * @fn Display::showWidgets
 * @brief Shows the message history when scrolling up.
 * @param[in] value current position of the vertical scroll bar
 */
void Display::showWidgets(int value) {
  if ((value == verticalScrollBar()->minimum()) and
      (message_counter < all_messages.length())) {
    /*! Add half of the maximum. */
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

/*!
 * @fn Display::removeMessage
 * @brief Removes the message from Display.
 * @attention This method does not remove the message from history.
 * @param[in,out] message
 */
void Display::removeMessage(Message *message) {
  messages_mutex.lock();
  message->close();
  vertical_box_layout->removeWidget(message);
  all_messages.removeOne(message);
  disconnect(message);
  message_counter--;
  messages_mutex.unlock();
}
