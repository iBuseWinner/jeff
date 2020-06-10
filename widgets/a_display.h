#ifndef A_DISPLAY_H
#define A_DISPLAY_H

#include <QScrollArea>
#include <QScrollBar>
#include <QVBoxLayout>
#include <QWidget>

/*!
 * Class: ADisplay.
 * Message widget.
 */
class ADisplay : public QScrollArea {
  Q_OBJECT
public:
  // Functions:
  explicit ADisplay(short _max_message_amount = 50, QWidget *parent = nullptr);
  void start();
  void addMessage(QWidget *message);

  /*! Sets the scroll state. */
  void setScrollEnabled(bool _scrollEnabled) { scrollEnabled = _scrollEnabled; }

private:
  Q_DISABLE_COPY(ADisplay)

  // Objects:
  QVBoxLayout *vertical_box_layout = nullptr;
  bool scrollEnabled = true;
  QList<QWidget *> all_messages;
  short message_counter = 0;
  short max_message_amount = 0;

  // Functions:
  void connector();
  void scrollDown(int min, int max);
  void scrollTumbler(int v);
  void showWidgets(int v = 0);
};

#endif // A_DISPLAY_H
