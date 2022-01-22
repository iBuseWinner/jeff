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
public:
  // Functions:
  /*!
   * @fn Display::setScrollEnabled
   * @brief Sets the scroll state.
   * @param[in] _scrollEnabled boolean value of whether scroll is enabled or not
   */
  void setScrollEnabled(bool _scrollEnabled) { scrollEnabled = _scrollEnabled; }

  // Functions described in 'display.cpp':
  explicit Display(short _max_message_amount = 50, QWidget *parent = nullptr);
  void start();
  void add_message(Message *message);
  void add_message_by_md(MessageData message_data);
  void add_message_with_widget(MessageData message_data, ModalHandler *handler);

private:
  Q_DISABLE_COPY(Display)

  // Objects:
  QVBoxLayout *vertical_box_layout = nullptr;
  bool scrollEnabled = true;
  QMutex messages_mutex;
  QList<QWidget *> all_messages;
  short message_counter = 0;
  short max_message_amount = 0;

  // Constants:
  inline static const QString object_name = "display";
  inline static const QString box_object_name = "box";
  inline static const QString box_style_sheet =
      "#display, #box { background-color: rgba(255, 255, 255, 0); }";

  // Functions described in 'display.cpp':
  void connector();
  void scrollDown(int min, int max);
  void scrollTumbler(int value);
  void showWidgets(int value = 0);
  void removeMessage(Message *message);
};

#endif
