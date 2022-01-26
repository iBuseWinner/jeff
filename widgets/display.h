#ifndef DISPLAY_H
#define DISPLAY_H

#include "core/model/message.h"
#include "dialogues/modal-handler.h"
#include "widgets/message.h"
#include <QMutex>
#include <QScrollArea>
#include <QScrollBar>
#include <QVBoxLayout>
#include <QWidget>

/*!
 * @class Display
 * @brief Widget that displays messages.
 * @sa Message
 */
class Display : public QScrollArea {
  Q_OBJECT
  Q_DISABLE_COPY(Display)
public:
  // Functions:
  /*!
   * @fn Display::set_scroll_enabled
   * @brief Sets the scroll state.
   * @param[in] _scroll_enabled boolean value of whether scroll is enabled or not
   */
  void set_scroll_enabled(bool _scroll_enabled) { scroll_enabled = _scroll_enabled; }

  // Functions described in 'display.cpp':
  Display(short _max_message_amount = 50, QWidget *parent = nullptr);
  void start();
  void add_message(Message *message);
  void add_message_by_md(MessageData message_data);
  void update_status(QPair<QString, MessageData> id_and_message_data);
  void add_message_with_widget(MessageData message_data, ModalHandler *handler);

private:
  // Objects:
  QVBoxLayout *vertical_box_layout = nullptr;
  bool scroll_enabled = true;
  QMutex messages_mutex;
  QList<Message *> all_messages;
  QMap<QString, Message *> status_messages;
  short message_counter = 0;
  short max_message_amount = 0;

  // Constants:
  inline static const QString object_name = "display";
  inline static const QString box_object_name = "box";
  inline static const QString box_style_sheet =
    "#display, #box { background-color: rgba(255, 255, 255, 0); }";

  // Functions described in 'display.cpp':
  void connector();
  void scroll_down(int min, int max);
  void scroll_tumbler(int value);
  void show_widgets(int value = 0);
  void remove_message(Message *message);
};

#endif
