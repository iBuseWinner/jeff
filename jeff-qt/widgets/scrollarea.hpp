#ifndef SCROLL_AREA_H
#define SCROLL_AREA_H

#include "widgets/styling.hpp"
#include "widgets/layouts/grid.hpp"
#include <QScrollArea>
#include <QScrollBar>
#include <QWidget>

/*! @class ScrollArea
 *  @brief TBD  */
class ScrollArea : public QScrollArea {
  Q_OBJECT
  Q_DISABLE_COPY(ScrollArea)
public:
  /*! @brief The constructor. */
  ScrollArea(QWidget *parent = nullptr) : QScrollArea(parent) {
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setFocusPolicy(Qt::NoFocus);
    setFrameStyle(QFrame::NoFrame);
    setFrameShadow(QFrame::Plain);
    setWidgetResizable(true);
    verticalScrollBar()->setStyleSheet(
      styling.css_scroll_bar.arg(styling.light_theme ? styling.css_light_sb : styling.css_dark_sb)
    );
    verticalScrollBar()->setFixedWidth(5);
  }
};

#endif
