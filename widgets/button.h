#ifndef BUTTON_H
#define BUTTON_H

#include "widgets/menu.h"
#include "widgets/styling.h"
#include <QFocusEvent>
#include <QTimer>
#include <QPoint>
#include <QPushButton>

/*! @class Button
 *  @brief Button with click animation and options.  */
class Button : public QPushButton {
  Q_OBJECT
  Q_DISABLE_COPY(Button)
public:
  // Functions described in `button.cpp`:
  Button(const QString &text = "", QWidget *parent = nullptr);
  void setMenu(Menu *menu);
};

#endif
