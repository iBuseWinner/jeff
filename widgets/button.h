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

protected:
  // Functions described in 'a_button.cpp':
  virtual void mousePressEvent(QMouseEvent *e) override;

private:
  Q_DISABLE_COPY(Button)

  // Objects:
  QColor currentButtonColor = unpressedButtonColor;

  // Constants:
  inline static const QColor unpressedButtonColor = QColor(255, 255, 255);
  inline static const QColor pressedButtonColor = QColor(0, 170, 255);

  // Functions:
  /*!
   * @fn Button::ss
   * @returns style sheet
   */
  QString ss() {
    return QString("Button {"
                   "border-width: 3px;"
                   "border-style: solid;"
                   "border-radius: 3px;"
                   "border-color: rgb(%1, %2, %3);"
                   "background-color: rgb(255, 255, 255);"
                   "color: rgb(0, 0, 0);"
                   "}")
        .arg(currentButtonColor.red())
        .arg(currentButtonColor.green())
        .arg(currentButtonColor.blue());
  }
};

#endif
