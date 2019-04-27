#ifndef A_PUSHBUTTON_H
#define A_PUSHBUTTON_H

#include <QAction>
#include <QFocusEvent>
#include <QObject>
#include <QPushButton>
#include <QStyle>

class APushButton : public QPushButton {
  Q_OBJECT

 public:
  APushButton(const QString &t, QWidget *p = nullptr);
  QString styleSheet();

 protected:
  virtual void focusInEvent(QFocusEvent *e) override;
  virtual void focusOutEvent(QFocusEvent *e) override;
  void updateBorderColor();

 private:
  Q_DISABLE_COPY(APushButton)
  const QColor lbc = QColor(255, 255, 255);
  const QColor nbc = QColor(0, 170, 255);
  QColor mbc = lbc;
};

#endif  // A_PUSHBUTTON_H
