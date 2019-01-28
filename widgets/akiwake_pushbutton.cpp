#include "akiwake_pushbutton.h"

AkiwakePushButton::AkiwakePushButton(const QString& Text, QWidget *parent)
    : QPushButton(parent) {
  // Creates a functional button.
  this->setStyleSheet(this->styleSheet());
  this->setText(Text);
  this->animation->setDuration(300);
  this->animation->setEasingCurve(QEasingCurve::InOutQuad);
  connect(this, &AkiwakePushButton::borderColorChanged, this,
          &AkiwakePushButton::setBorderColor);
}

AkiwakePushButton::~AkiwakePushButton() {
  // Prevents memory leaks.
  delete this->animation;
}

QString AkiwakePushButton::styleSheet() {
  // Returns the main style sheet.
  return QString("AkiwakePushButton {"
                 "border-width: 3px;"
                 "border-style: solid;"
                 "border-radius: 3px;"
                 "border-color: rgb(%1, %2, %3);"
                 "background-color: rgb(255, 255, 255);"
                 "color: rgb(0, 0, 0);"
                 "}"
                 "AkiwakePushButton[theme=\"dark\"] {"
                 "border-color: rgb(%4, %5, %6);"
                 "background-color: rgb(0, 0, 0);"
                 "color: rgb(255, 255, 255);"
                 "}")
      .arg(m_border_color.red())
      .arg(m_border_color.green())
      .arg(m_border_color.blue())
      .arg(255 - m_border_color.red())
      .arg(255 - m_border_color.green())
      .arg(255 - m_border_color.blue());
}

void AkiwakePushButton::focusInEvent(QFocusEvent *e) {
  // Animation block.
  if (this->animation->state() == QAbstractAnimation::Running)
    this->animation->stop();
  this->animation->setTargetObject(this);
  this->animation->setPropertyName("border_color");
  this->animation->setStartValue(m_border_color);
  this->animation->setEndValue(QColor(0, 170, 255));
  this->animation->start();
  e->accept();
}

void AkiwakePushButton::focusOutEvent(QFocusEvent *e) {
  // Animation block.
  if (this->animation->state() == QAbstractAnimation::Running)
    this->animation->stop();
  this->animation->setTargetObject(this);
  this->animation->setPropertyName("border_color");
  this->animation->setStartValue(m_border_color);
  this->animation->setEndValue(l_border_color);
  this->animation->start();
  e->accept();
}

void AkiwakePushButton::setBorderColor() {
  // Animation tools.
  this->setStyleSheet(this->styleSheet());
  this->style()->unpolish(this);
  this->style()->polish(this);
  this->update();
}
