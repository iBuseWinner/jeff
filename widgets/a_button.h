#ifndef A_PUSHBUTTON_H
#define A_PUSHBUTTON_H

#include <QFocusEvent>
#include <QToolButton>

class AButton : public QToolButton {
  Q_OBJECT

 public:
  AButton(const QString &t = "", QWidget *p = nullptr);

 protected:
  virtual void mousePressEvent(QMouseEvent *e) override;
  virtual void mouseReleaseEvent(QMouseEvent *e) override;

 private:
  Q_DISABLE_COPY(AButton)
  const QColor lbc = QColor(255, 255, 255);
  const QColor nbc = QColor(0, 170, 255);
  QColor mbc = lbc;
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
        .arg(mbc.red())
        .arg(mbc.green())
        .arg(mbc.blue());
  }
};

#endif  // A_PUSHBUTTON_H
