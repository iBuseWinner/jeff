#ifndef DISPLAY_H
#define DISPLAY_H

#include "core-kit/history-processor.h"
#include "core-kit/model/message.h"
#include "dialogues/modal-handler.h"
#include "maddy/parser.h"
#include "widgets/message.h"
#include "widgets/scrollarea.h"
#include "widgets/layouts/linears.h"
#include <memory>
#include <QWidget>

/*! @class Display
 *  @brief Widget that displays messages.
 *  @sa Message  */
class Display : public ScrollArea {
  Q_OBJECT
  Q_DISABLE_COPY(Display)
public:
  // Functions:
  /*! @brief Sets the scroll state. */
  void set_scroll_enabled(bool _scroll_enabled) { scroll_enabled = _scroll_enabled; }
  ~Display() { delete markdown_parser; }

  // Functions described in `display.cpp`:
  Display(HProcessor *_hp = nullptr, short _max_message_amount = 50, QWidget *parent = nullptr);
  void start();
  void start_by(MessagesMeta history);
  void add_message(Message *message);
  void add_message_by_md(MessageMeta message_data);
  void update_status(QPair<QString, MessageMeta> id_and_message_data);
  void add_message_with_widget(MessageMeta message_data, ModalHandler *handler);

private:
  // Objects:
  maddy::Parser *markdown_parser = nullptr;
  HProcessor *hp = nullptr;
  VLineLt *vt_layout = nullptr;
  QSpacerItem *spacer = nullptr;
  bool scroll_enabled = true;
  QList<Message *> all_messages;
  QMap<QString, Message *> status_messages;
  short message_counter = 0;
  short max_message_amount = 0;

  // Constants:
  const char *object_name = "display";
  const char *box_object_name = "box";
  const char *box_style_sheet = "#display, #box { background-color: rgba(255, 255, 255, 0); }";

  // Functions described in `display.cpp`:
  void scroll_down(int min, int max);
  void scroller(int value);
  void prepare_message(Message *message);
  void remove_message(Message *message);
  void resizeEvent(QResizeEvent *event) override;
  void fit_messages();
};

#endif
