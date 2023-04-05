#include "combobox.hpp"

/*! @brief The constructor. */
ComboBox::ComboBox(QWidget *parent) : QComboBox(parent) {
  setFocusPolicy(Qt::NoFocus);
  setStyleSheet(
    QString("ComboBox { background-color: %1; color: %2; } ")
      .arg(styling.css_bg_color).arg(styling.css_fg_color) + 
    styling.css_scroll_bar.arg(styling.light_theme ? styling.css_light_sb : styling.css_dark_sb)
  );
}
