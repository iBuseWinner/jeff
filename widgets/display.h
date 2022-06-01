#ifndef DISPLAY_H
#define DISPLAY_H

#include "core/history-processor.h"
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
  /*! @brief Sets the scroll state. */
  void set_scroll_enabled(bool _scroll_enabled) { scroll_enabled = _scroll_enabled; }

  // Functions described in `display.cpp`:
  Display(HProcessor *_hp = nullptr, short _max_message_amount = 50, QWidget *parent = nullptr);
  void start();
  void start_by(Messages history);
  void add_message(Message *message);
  void add_message_by_md(MessageData message_data);
  void update_status(QPair<QString, MessageData> id_and_message_data);
  void add_message_with_widget(MessageData message_data, ModalHandler *handler);

private:
  // Objects:
  HProcessor *hp = nullptr;
  QVBoxLayout *vertical_box_layout = nullptr;
  QSpacerItem *spacer = nullptr;
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

  // Functions described in `display.cpp`:
  void scroll_down(int min, int max);
  void scroller(int value);
  void prepare_message(Message *message);
  void remove_message(Message *message);
  void resizeEvent(QResizeEvent *event) override;
  void fit_messages();
};

#endif
