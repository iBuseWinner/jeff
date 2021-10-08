#include "button.h"

/*!
 * @fn Button::Button
 * @brief The constructor.
 * @param[in] text button text
 * @param[in,out] parent QObject parent
 */
Button::Button(const QString &text, QWidget *parent) : QToolButton(parent) {
  setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
  setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  setFocusPolicy(Qt::NoFocus);
  setStyleSheet(ss());
  setAutoRaise(true);
  setText(text);
}

/*!
 * @fn Button::mousePressEvent
 * @brief Responsible for the style of the pressed button.
 * @param[in,out] e mouse event
 */
void Button::mousePressEvent(QMouseEvent *e) {
  currentButtonColor = pressedButtonColor;
  setStyleSheet(ss());
  QToolButton::mousePressEvent(e);
  QTimer::singleShot(200, this, [this] {
    currentButtonColor = unpressedButtonColor;
    setStyleSheet(ss());
  });
}
