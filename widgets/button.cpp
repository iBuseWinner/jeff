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
  setAutoRaise(true);
  setText(text);
  setStyleSheet(QString(
    "Button { text-align: center; border-width: 3px; border-style: solid; border-radius: 3px;"
    "border-color: %1; background-color: %1; color: %2; } Button:pressed, Button:open, "
    "Button::menu-arrow:pressed { color: %2; border-color: rgb(0, 170, 255); }"
    "Button[popupMode=\"2\"] {" /* padding-right: 20px; text-align: center; +*/ " color: %2; }"
    "Button::menu-indicator { image: none; }"
  ).arg(styling.css_bg_color).arg(styling.css_fg_color));
}
