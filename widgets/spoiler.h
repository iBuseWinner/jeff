#ifndef SPOILER_H
#define SPOILER_H

#include <QFrame>
#include <QGridLayout>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>
#include <QScrollArea>
#include <QToolButton>
#include <QWidget>

/*!
 * @class Spoiler
 * @brief Collapses and expands a large amount of content.
 * @copyright https://stackoverflow.com/users/1174343/x-squared
 * @details @link https://stackoverflow.com/questions/32476006/how-to-make-an-expandable-collapsable-section-widget-in-qt
 */
class Spoiler : public QWidget {
  Q_OBJECT
  Q_DISABLE_COPY(Spoiler)
public:
  // Functions described in `spoiler.cpp`:
  Spoiler(const QString &title = "", const int _animation_duration = 300, QWidget *parent = nullptr);
  void set_title(QString title);
  void set_content_layout(QLayout *content_layout);
  
private:
  // Objects:
  QGridLayout main_layout;
  QToolButton toggle_button;
  QFrame header_line;
  QParallelAnimationGroup toggle_animation;
  QScrollArea content_area;
  int animation_duration = 300;
};

#endif
