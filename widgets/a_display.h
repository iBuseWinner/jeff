#ifndef A_DISPLAY_H
#define A_DISPLAY_H

#include "widgets/a_message.h"
#include <QMutex>
#include <QScrollArea>
#include <QScrollBar>
#include <QVBoxLayout>
#include <QWidget>

/*!
 * @class ADisplay
 * @brief Widget that displays messages.
 * @sa AMessage
 */
class ADisplay : public QScrollArea {
  Q_OBJECT
public:
  // Functions:
  /*!
   * @fn ADisplay::setScrollEnabled
   * @brief Sets the scroll state.
   * @param[in] _scrollEnabled boolean value of whether scroll is enabled or not
   */
  void setScrollEnabled(bool _scrollEnabled) { scrollEnabled = _scrollEnabled; }

  // Functions described in 'a_display.cpp':
  explicit ADisplay(short _max_message_amount = 50, QWidget *parent = nullptr);
  void start();
  void addMessage(AMessage *message);

private:
  Q_DISABLE_COPY(ADisplay)

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

  // Functions described in 'a_display.cpp':
  void connector();
  void scrollDown(int min, int max);
  void scrollTumbler(int value);
  void showWidgets(int value = 0);
  void removeMessage(AMessage *message);
};

#endif // A_DISPLAY_H
