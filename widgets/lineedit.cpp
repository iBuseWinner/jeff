#include "lineedit.h"

/*!
 * @fn LineEdit::LineEdit
 * @brief The constructor.
 * @param[in,out] parent QObject parent
 */
LineEdit::LineEdit(QWidget *parent) : QLineEdit(parent) {
  setStyleSheet(style_sheet);
  setFrame(false);
}
