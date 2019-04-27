#include "a_pushbutton.h"

APushButton::APushButton(const QString &t, QWidget *p) : QPushButton(p) {
  // Creates a functional button.
  setStyleSheet(styleSheet());
  setText(t);
}

QString APushButton::styleSheet() {
  // Returns the main style sheet.
  return QString(
             "APushButton {"
             "border-width: 3px;"
             "border-style: solid;"
             "border-radius: 3px;"
             "border-color: rgb(%1, %2, %3);"
             "background-color: rgb(255, 255, 255);"
             "color: rgb(0, 0, 0);"
             "}")
      .arg(mbc.red())
      .arg(mbc.green())
      .arg(mbc.blue());
}

void APushButton::focusInEvent(QFocusEvent *e) {
  // Animation block.
  mbc = nbc;
  updateBorderColor();
  e->accept();
}

void APushButton::focusOutEvent(QFocusEvent *e) {
  mbc = lbc;
  updateBorderColor();
  e->accept();
}

void APushButton::updateBorderColor() {
  // Animation tools.
  setStyleSheet(styleSheet());
  style()->unpolish(this);
  style()->polish(this);
  update();
}
