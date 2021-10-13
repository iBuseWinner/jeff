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
public:
  // Functions described in 'a_button.cpp':
  explicit Button(const QString &text = "", QWidget *parent = nullptr);
private:
  Q_DISABLE_COPY(Button)
};

#endif
