#include "menu.h"

/*!
 * @fn Menu::Menu
 * @brief The constructor.
 */
Menu::Menu() /* : QMenu(nullptr)*/ {
  setStyleSheet(style_sheet);
}

/*!
 * @fn Menu::Menu
 * @brief The constructor.
 */
Menu::Menu(QWidget *parent = nullptr) : QMenu(parent) {
  setStyleSheet(style_sheet);
}
