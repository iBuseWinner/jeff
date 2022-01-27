#ifndef MENU_H
#define MENU_H

#include "widgets/styling.h"
#include <QMenu>

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
  // Constants:
  inline static const QString style_sheet =
    "QMenu { padding: 3px; border: 2px solid transparent; border-radius: 7px;"
    "background-color: %1; color: %2; } QMenu::item { border: 2px solid transparent;"
    "border-radius: 5px; padding: 5px 0px 5px 3px; } QMenu::item:selected { background-color: %3; }"
    "QMenu::item:pressed { background-color: %4; } QMenu::icon { padding-left: 3px; }";
};

#endif
