#include "a_button.h"

/*!
 * @fn AButton::AButton
 * @brief The constructor.
 * @param[in] text button text
 * @param[in,out] parent QObject parent
 */
AButton::AButton(const QString &text, QWidget *parent) : QToolButton(parent) {
  setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
  setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  setFocusPolicy(Qt::NoFocus);
  setStyleSheet(ss());
  setAutoRaise(true);
  setText(text);
}

/*!
 * @fn AButton::mousePressEvent
 * @brief Responsible for the style of the pressed button.
 * @param[in,out] e mouse event
 */
void AButton::mousePressEvent(QMouseEvent *e) {
  currentButtonColor = pressedButtonColor;
  setStyleSheet(ss());
  QToolButton::mousePressEvent(e);
  QTimer::singleShot(200, this, [this] {
    currentButtonColor = unpressedButtonColor;
    setStyleSheet(ss());
  });
}
