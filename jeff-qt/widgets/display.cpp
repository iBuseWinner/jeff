#include "display.hpp"

/*! @brief The constructor. */
Display::Display(HProcessor *_hp, short _max_message_amount, QWidget *parent)
    : ScrollArea(parent), hp(_hp), max_message_amount(_max_message_amount) {
  std::shared_ptr<maddy::ParserConfig> markdown_config = std::make_shared<maddy::ParserConfig>();
  markdown_config->isHTMLWrappedInParagraph = false;
  markdown_parser = new maddy::Parser(markdown_config);
  connect(verticalScrollBar(), &QScrollBar::rangeChanged, this, &Display::scroll_down);
  connect(verticalScrollBar(), &QScrollBar::valueChanged, this, &Display::scroller);
  setObjectName(object_name);
  setStyleSheet(box_style_sheet);
  start();
}

/*! @brief Customizes the message before showing it. */
void Display::prepare_message(Message *message) {
  message->setParent(this);
  connect(message, &Message::closed, this, [this, message] { remove_message(message); });
  auto max_width = int(width() * 0.8);
  message->setWidth(max_width);
}

/*! @brief Really adds a message to the display. @sa ..._by_md and ..._with_widget. */
void Display::add_message(Message *message) {
  prepare_message(message);
  message_counter++;
  all_messages.append(message);
  vt_layout->addw(message);
  /*! If the scroll is approximately below the middle, and the number of
   *  displayed messages is greater than the maximum...  */
  if ((verticalScrollBar()->value() >
       ((verticalScrollBar()->minimum() + verticalScrollBar()->maximum()) / 2)) and
      (message_counter > max_message_amount))
    /*! ...we delete all unnecessary messages */
    while (message_counter > max_message_amount) {
      auto *message = all_messages.takeAt(0);
      for (auto key : status_messages.keys()) 
        if (status_messages[key] == message) status_messages.remove(key);
      message->hide();
      vt_layout->rem(message);
      delete message;
      message_counter--;
    }
}

/*! @brief Constructs @a Message and adds it to the display. */
void Display::add_message_by_md(MessageMeta message_data) {
  add_message(new Message(markdown_parser, message_data));
}

/*! @brief Updates the text of a message by its ID, or adds a new message with that ID. */
void Display::update_status(QPair<QString, MessageMeta> id_and_message_data) {
  if (not status_messages.contains(id_and_message_data.first)) {
    auto *message = new Message(markdown_parser, id_and_message_data.second);
    add_message(message);
    status_messages[id_and_message_data.first] = message;
  } else {
    status_messages[id_and_message_data.first]->update_text(id_and_message_data.second.content);
  }
}

/*! @brief Constructs @a Message and adds it to the display. */
void Display::add_message_with_widget(MessageMeta message_data, ModalHandler *handler) {
  auto *message = new Message(markdown_parser, message_data);
  handler->getPrisoner()->setParent(message);
  message->widget(handler);
  add_message(message);
}

/*! @brief Sets the widget to its initial state.
 *  @details The method is also used to reset an existing state.  */
void Display::start() {
  spacer = new QSpacerItem(0, 0, QSizePolicy::Fixed, QSizePolicy::Expanding);
  message_counter = 0;
  all_messages.clear();
  status_messages.clear();
  if (vt_layout) delete vt_layout;
  auto *w = takeWidget();
  if (w) delete w;
  vt_layout = VLineLt::another()
    ->spacing()->addi(new QSpacerItem(0, 0, QSizePolicy::Fixed, QSizePolicy::Expanding));
  QWidget *box = new QWidget(this);
  box->setContentsMargins(5, 0, 5, 0);
  box->setObjectName(box_object_name);
  box->setLayout(vt_layout);
  setWidget(box);
}

/*! @details Of the given message history, it only shows the last 50. This is because the widget
 *  is designed to only show 50 (or whatever) messages when autoscroll is enabled.  */
void Display::start_by(MessagesMeta history) {
  start();
  scroll_enabled = true;
  int m /*!< @brief Max messages to be shown now. */ = history.length() > max_message_amount ?
    max_message_amount : history.length();
  int l = history.length();
  for (int i = 0; i < l - m; i++) {
    auto *message = new Message(markdown_parser, history[i]);
    prepare_message(message);
    message_counter++;
    all_messages.append(message);
  }
  for (int i = l - m; i < l; i++) add_message_by_md(history[i]);
}

/*! @brief When the scrolling range of the display changes, it scrolls to the end
 *  if automatic scrolling is enabled.  */
void Display::scroll_down(int min, int max) {
  Q_UNUSED(min)
  if (scroll_enabled) verticalScrollBar()->setValue(max);
}

/*! @brief Enables or disables automatic scrolling and shows the message history when scrolling up. */
void Display::scroller(int value) {
  if (not scroll_enabled and (value == verticalScrollBar()->maximum())) {
    scroll_enabled = true;
    if (not spacer) {
      spacer = new QSpacerItem(0, 0, QSizePolicy::Fixed, QSizePolicy::Expanding);
      vt_layout->insertItem(0, spacer);
    }
  }
  else if (scroll_enabled and (viewportSizeHint().height() > height()) and (value != verticalScrollBar()->maximum())) {
    scroll_enabled = false;
    if (spacer) {
      vt_layout->removeItem(spacer);
      delete spacer;
      spacer = nullptr;
    }
  }
  auto length = hp->length();
  if ((value == verticalScrollBar()->minimum()) and (message_counter < length)) {
    /*! Add half of the maximum. */
    short portion = max_message_amount / 2;
    if (message_counter + portion > length) portion = length - message_counter;
    auto *messages = hp->messages();
    while (portion--) {
      auto *message = new Message(markdown_parser, messages->at(messages->length() - message_counter - 1));
      all_messages.insert(0, message);
      int pos = 0;
      if (spacer) pos = 1;
      vt_layout->insertWidget(pos, message);
      message->show();
      message_counter++;
    }
    verticalScrollBar()->setValue(1);
  }
}

/*! @brief Removes the message from Display. */
void Display::remove_message(Message *message) {
  message->close();
  vt_layout->rem(message);
  all_messages.removeOne(message);
  disconnect(message);
  message_counter--;
}

/*! @brief When resizing, calls the @a fit_messages function again. */
void Display::resizeEvent(QResizeEvent *event) {
  fit_messages();
  QScrollArea::resizeEvent(event);
}

/*! @brief Controls the size of messages on the screen, limiting them to 80% of the display width. */
void Display::fit_messages() {
  auto max_width = int(width() * 0.8);
  for (auto *message : all_messages) message->setWidth(max_width);
}
