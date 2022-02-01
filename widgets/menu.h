#ifndef MENU_H
#define MENU_H

#include "widgets/styling.h"
#include <QBitmap>
#include <QBrush>
#include <QMenu>
#include <QPainter>
#include <QPixmap>

/*!
 * @class Menu
 * @brief Menu with a special style of design.
 */
class Menu : public QMenu {
  Q_OBJECT
  Q_DISABLE_COPY(Menu)
public:
  // Functions described in `menu.cpp`:
  Menu();
  Menu(QWidget *parent);

private:
  // Functions described in `menu.cpp`:
  void setup();
};

#endif
