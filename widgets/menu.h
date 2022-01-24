#ifndef MENU_H
#define MENU_H

#include <QMenu>

/*!
 * @class Menu
 * @brief Menu with a special style of design.
 */
class Menu : public QMenu {
  Q_OBJECT
  // Q_DISABLE_COPY(Menu)
public:
  // Functions described in `menu.cpp`:
  Menu();
  Menu(QWidget *parent);
  
private:
  // Constants:
  inline static const QString style_sheet =
    "QMenu { padding: 3px; border: 2px solid transparent; border-radius: 7px;"
    "background-color: rgb(40, 44, 52); color: rgb(255, 255, 255); } QMenu::item {"
    "border: 2px solid transparent; border-radius: 5px; padding: 5px 5px; } "
    "QMenu::item:selected { background-color: rgb(89, 92, 102); } "
    "QMenu::item:pressed { background-color: rgb(110, 113, 126); }";
};

#endif
