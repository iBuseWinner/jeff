#include "a_lineedit.h"

/*!
 * @fn ALineEdit::ALineEdit
 * @brief The constructor.
 * @param[in,out] parent QObject parent
 */
ALineEdit::ALineEdit(QWidget *parent) : QLineEdit(parent) {
  setStyleSheet(style_sheet);
  setFrame(false);
}
