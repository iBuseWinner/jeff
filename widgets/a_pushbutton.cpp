#include "a_pushbutton.h"

APushButton::APushButton(const QString& Text, QWidget *parent)
    : QPushButton(parent) {
  // Creates a functional button.
  this->setStyleSheet(this->styleSheet());
  this->setText(Text);
}

QString APushButton::styleSheet() {
  // Returns the main style sheet.
  return QString("APushButton {"
                 "border-width: 3px;"
                 "border-style: solid;"
                 "border-radius: 3px;"
                 "border-color: rgb(%1, %2, %3);"
                 "background-color: rgb(255, 255, 255);"
                 "color: rgb(0, 0, 0);"
                 "}")
      .arg(m_border_color.red())
      .arg(m_border_color.green())
      .arg(m_border_color.blue());
}

void APushButton::focusInEvent(QFocusEvent *e) {
  // Animation block.
  this->m_border_color = QColor(0, 170, 255);
  this->updateBorderColor();
  e->accept();
}

void APushButton::focusOutEvent(QFocusEvent *e) {
  this->m_border_color = this->l_border_color;
  this->updateBorderColor();
  e->accept();
}

void APushButton::updateBorderColor() {
  // Animation tools.
  this->setStyleSheet(this->styleSheet());
  this->style()->unpolish(this);
  this->style()->polish(this);
  this->update();
}
