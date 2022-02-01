#include "menu.h"

/*! @brief The constructor. */
Menu::Menu() { setup(); }

/*! @brief The constructor. */
Menu::Menu(QWidget *parent = nullptr) : QMenu(parent) { setup(); }

/*! @brief Sets the menu style. */
void Menu::setup() {
  setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
  setWindowFlags(windowFlags() | Qt::NoDropShadowWindowHint);
  setAttribute(Qt::WA_TranslucentBackground);
  setStyleSheet(
    QString(
      "Menu { padding: 3px; border: 2px solid transparent; border-radius: 7px;"
      "background-color: %1; color: %2; } Menu::item { border: 2px solid transparent;"
      "border-radius: 5px; padding: 5px 0px 5px 3px; } Menu::item:selected { background-color: %3; }"
      "Menu::item:pressed { background-color: %4; } Menu::icon { padding-left: 3px; }")
    .arg(styling.css_bg_color).arg(styling.css_fg_color)
    .arg(styling.light_theme ? styling.css_light_sm : styling.css_dark_sm)
    .arg(styling.light_theme ? styling.css_light_pm : styling.css_dark_pm)
  );
}
