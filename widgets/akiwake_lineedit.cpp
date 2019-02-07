#include "akiwake_lineedit.h"

AkiwakeLineEdit::AkiwakeLineEdit(QWidget *parent) : QLineEdit(parent) {
  int ID =
      QFontDatabase::addApplicationFont(":/arts/fonts/iosevka-regular.ttf");
  QFont Font = QFontDatabase::applicationFontFamilies(ID).at(0);
  this->setFont(Font);
  this->setStyleSheet("color: rgb(0, 0, 0);");
  this->setFrame(0);
}
