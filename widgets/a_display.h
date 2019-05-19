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
  explicit ADisplay(QWidget *parent = nullptr);
  void start();

  /*! Adds a message to the display. */
  void addMessage(QWidget *message) { lt->addWidget(message); }

  /*! Sets the scroll state. */
  void setScrollEnabled(bool _scrollEnabled) { scrollEnabled = _scrollEnabled; }

 private:
  Q_DISABLE_COPY(ADisplay)

  // Objects:
  QVBoxLayout *lt = nullptr;
  bool scrollEnabled = true;

  // Functions:
  void connector();
  void scrollDown(int min, int max);
  void scrollTumbler(int v);
};

#endif  // A_DISPLAY_H
