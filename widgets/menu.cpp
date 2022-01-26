#include "menu.h"

/*! @brief The constructor. */
Menu::Menu() {
  setStyleSheet(
    style_sheet.arg(styling.css_bg_color).arg(styling.css_fg_color)
    .arg(styling.light_theme ? styling.css_light_sm : styling.css_dark_sm)
    .arg(styling.light_theme ? styling.css_light_pm : styling.css_dark_pm)
  );
}

/*! @brief The constructor. */
Menu::Menu(QWidget *parent = nullptr) : QMenu(parent) {
  setStyleSheet(
    style_sheet.arg(styling.css_bg_color).arg(styling.css_fg_color)
    .arg(styling.light_theme ? styling.css_light_sm : styling.css_dark_sm)
    .arg(styling.light_theme ? styling.css_light_pm : styling.css_dark_pm)
  );
}
