#include "a_lineedit.h"

/*!
 * Argument: QWidget {*parent}.
 * Creates an ALineEdit.
 */
ALineEdit::ALineEdit(QWidget *parent) : QLineEdit(parent) {
  setStyleSheet("color: rgb(0, 0, 0);");
  setFrame(false);
}
