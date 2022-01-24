#ifndef BUTTON_H
#define BUTTON_H

#include <QFocusEvent>
#include <QTimer>
#include <QToolButton>

/*!
 * @class Button
 * @brief Button with click animation and options.
 */
class Button : public QToolButton {
  Q_OBJECT
  Q_DISABLE_COPY(Button)
public:
  // Functions described in 'button.cpp':
  Button(const QString &text = "", QWidget *parent = nullptr);
};

#endif
