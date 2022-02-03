#include "display.h"

/*!
 * @fn Display::Display
 * @brief The constructor.
 * @param[in] _max_message_amount number of messages that can be displayed simultaneously
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
  connect(message, &Message::closed, this, [this, message] { remove_message(message); });
  auto max_width = int(width() * 0.8);
  message->setWidth(max_width);
  all_messages.append(message);
  vertical_box_layout->addWidget(message);
  /*! If the scroll is approximately below the middle, and the number of
   *  displayed messages is greater than the maximum...  */
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
 * @fn Display::add_message_by_md
 * @brief Constructs @a Message and adds it to the display.
 * @param[in] message_data message to be added
 */
void Display::add_message_by_md(MessageData message_data) {
  add_message(new Message(message_data));
}

/*!
 * @fn Display::update_status
 * @brief Updates the text of a message by its ID, or adds a new message with that ID.
 * @param[in] id_and_message_data paired
 */
void Display::update_status(QPair<QString, MessageData> id_and_message_data) {
  if (not status_messages.contains(id_and_message_data.first)) {
    auto *message = new Message(id_and_message_data.second);
    add_message(message);
    messages_mutex.lock();
    status_messages[id_and_message_data.first] = message;
    messages_mutex.unlock();
  } else {
    messages_mutex.lock();
    status_messages[id_and_message_data.first]->update_text(id_and_message_data.second.content);
    messages_mutex.unlock();
  }
}

/*!
 * @fn Display::add_message_with_widget
 * @brief Constructs @a Message and adds it to the display.
 * @param[in] message_data message to be added
 * @param[in,out] handler modal handler with @a QWidget to be shown
 */
void Display::add_message_with_widget(MessageData message_data, ModalHandler *handler) {
  auto *message = new Message(message_data);
  handler->getPrisoner()->setParent(message);
  message->widget(handler);
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
  vertical_box_layout = new QVBoxLayout(this);
  vertical_box_layout->setSpacing(0);
  vertical_box_layout->setMargin(0);
  vertical_box_layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Fixed, QSizePolicy::Expanding));
  box->setLayout(vertical_box_layout);
  setWidget(box);
  messages_mutex.unlock();
  verticalScrollBar()->setStyleSheet(
    styling.css_scroll_bar.arg(styling.light_theme ? styling.css_light_sb : styling.css_dark_sb)
  );
  verticalScrollBar()->setFixedWidth(5);
}

/*!
 * @fn Display::connector
 * @brief Establishes communications for user interaction through the widget.
 */
void Display::connector() {
  connect(verticalScrollBar(), &QScrollBar::rangeChanged, this, &Display::scroll_down);
  connect(verticalScrollBar(), &QScrollBar::valueChanged, this, &Display::scroll_tumbler);
  connect(verticalScrollBar(), &QScrollBar::valueChanged, this, &Display::show_widgets);
}

/*!
 * @fn Display::scroll_down
 * @brief When the scrolling range of the display changes, it scrolls to the end
 * if automatic scrolling is enabled.
 * @param[in] min minimal vertical scroll bar value
 * @param[in] max maximal vertical scroll bar value
 */
void Display::scroll_down(int min, int max) {
  Q_UNUSED(min)
  if (scroll_enabled) verticalScrollBar()->setValue(max);
}

/*!
 * @fn Display::scroll_tumbler
 * @brief Enables or disables automatic scrolling.
 * @param[in] value current position of the vertical scroll bar
 */
void Display::scroll_tumbler(int value) {
  if (not scroll_enabled and (value == verticalScrollBar()->maximum())) scroll_enabled = true;
  else if (scroll_enabled and (value != verticalScrollBar()->maximum())) scroll_enabled = false;
}

/*!
 * @fn Display::show_widgets
 * @brief Shows the message history when scrolling up.
 * @param[in] value current position of the vertical scroll bar
 */
void Display::show_widgets(int value) {
  if ((value == verticalScrollBar()->minimum()) and (message_counter < all_messages.length())) {
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

/*! @brief Removes the message from Display. */
void Display::remove_message(Message *message) {
  messages_mutex.lock();
  message->close();
  vertical_box_layout->removeWidget(message);
  all_messages.removeOne(message);
  disconnect(message);
  message_counter--;
  messages_mutex.unlock();
}

/*! @brief TBD */
void Display::resizeEvent(QResizeEvent *event) { fit_messages(); event->accept(); }

/*! @brief TBD */
void Display::fit_messages() {
  if (not messages_mutex.try_lock()) { return; };
  auto max_width = int(width() * 0.8);
  for (auto *message : all_messages) message->setWidth(max_width);
  messages_mutex.unlock();
}
