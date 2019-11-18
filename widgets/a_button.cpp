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
  QTimer::singleShot(200, this, [this] {
    currentButtonColor = unpressedButtonColor;
    setStyleSheet(ss());
  });
}
