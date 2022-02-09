#ifndef SCROLL_FREEZER_WIDGET_H
#define SCROLL_FREEZER_WIDGET_H

#include <QWheelEvent>
#include <QWidget>

/*!
 * @class ScrollFreezerWidget
 * @brief Some widget messages have scroll areas, and this widget allows you to prevent the Display 
 * from scrolling while the mouse cursor is over this widget.
 */
class ScrollFreezerWidget : public QWidget {
  Q_OBJECT
  Q_DISABLE_COPY(ScrollFreezerWidget)
public:
  // Functions:
  /*! @brief The constructor. */
  ScrollFreezerWidget(QWidget *parent = nullptr) : QWidget(parent) {}
  /*! @brief Accepts the event, which prevents the event from passing to parent widgets. */
  void wheelEvent(QWheelEvent *event) { event->accept(); }
};

#endif
