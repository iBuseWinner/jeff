#include "a_lineedit.h"

ALineEdit::ALineEdit(QWidget *p) : QLineEdit(p) {
  setStyleSheet("color: rgb(0, 0, 0);");
  setFrame(false);
}
