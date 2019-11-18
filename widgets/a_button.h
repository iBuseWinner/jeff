#ifndef A_PUSHBUTTON_H
#define A_PUSHBUTTON_H

#include <QFocusEvent>
#include <QTimer>
#include <QToolButton>

/*!
 * Class: AButton.
 * Button with click animation and options.
 */
class AButton : public QToolButton {
  Q_OBJECT

 public:
  // Functions:
  AButton(const QString &text = "", QWidget *parent = nullptr);

 protected:
  virtual void mousePressEvent(QMouseEvent *e) override;

 private:
  Q_DISABLE_COPY(AButton)

  // Objects:
  const QColor unpressedButtonColor = QColor(255, 255, 255);
  const QColor pressedButtonColor = QColor(0, 170, 255);
  QColor currentButtonColor = unpressedButtonColor;

  // Functions:
  /*! Returns the style sheet. */
  QString ss() {
    return QString(
               "AButton {"
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

#endif  // A_PUSHBUTTON_H
