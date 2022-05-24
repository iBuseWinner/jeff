#include "lineedit.h"

/*! @brief The constructor. */
LineEdit::LineEdit(QWidget *parent) : QLineEdit(parent) {
  setStyleSheet(
    style_sheet.arg(styling.css_bg_color).arg(styling.css_fg_color).arg(styling.css_hl_color)
  );
  setPlaceholderText(tr("Enter your message..."));
  setContextMenuPolicy(Qt::NoContextMenu);
  setFrame(false);
}

/*! @brief Sets a different widget style sheet so that its border is not displayed on the screen. */
void LineEdit::hide_border() {
  setStyleSheet(
    wob_style_sheet.arg(styling.css_bg_color).arg(styling.css_fg_color)
  );
}
