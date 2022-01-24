#include "widgets/menu.h"

/*!
 * @fn Menu::Menu
 * @brief The constructor.
 */
Menu::Menu(QWidget *parent = nullptr) : QMenu(parent) {
  setStyleSheet(
    "QMenu { padding: 3px; border: 2px solid transparent; border-radius: 7px;"
    "background-color: rgb(40, 44, 52); color: rgb(255, 255, 255); } QMenu::item {"
    "border: 2px solid transparent; border-radius: 5px; padding: 5px 5px; } "
    "QMenu::item:selected { background-color: rgb(89, 92, 102); } "
    "QMenu::item:pressed { background-color: rgb(110, 113, 126); }"
  );
}
