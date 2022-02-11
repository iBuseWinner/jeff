#include "lineedit.h"

/*! @brief The constructor. */
LineEdit::LineEdit(QWidget *parent) : QLineEdit(parent) {
  setStyleSheet(
    style_sheet.arg(styling.css_bg_color).arg(styling.css_fg_color)
  );
  setPlaceholderText(tr("Enter your message..."));
  setContextMenuPolicy(Qt::NoContextMenu);
  setFrame(false);
}
