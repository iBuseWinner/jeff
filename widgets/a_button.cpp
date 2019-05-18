#include "a_button.h"

/*!
 * Arguments: QString {text} [button text],
 *            QWidget {parent}.
 * Creates an AButton.
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
 * Argument: QMouseEvent {*e} [transmitted automatically by Qt].
 * Responsible for the style of the pressed button.
 */
void AButton::mousePressEvent(QMouseEvent *e) {
  currentButtonColor = pressedButtonColor;
  setStyleSheet(ss());
  QToolButton::mousePressEvent(e);
}

/*!
 * Argument: QMouseEvent {*e} [transmitted automatically by Qt].
 * Responsible for the style of the unpressed button.
 */
void AButton::mouseReleaseEvent(QMouseEvent *e) {
  currentButtonColor = unpressedButtonColor;
  setStyleSheet(ss());
  QToolButton::mouseReleaseEvent(e);
}
