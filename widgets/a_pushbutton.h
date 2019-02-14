#ifndef A_PUSHBUTTON_H
#define A_PUSHBUTTON_H

#include <QAbstractAnimation>
#include <QAction>
#include <QFocusEvent>
#include <QObject>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QStyle>

class APushButton : public QPushButton {
  Q_OBJECT

public:
  APushButton(const QString& Text, QWidget *parent = nullptr);
  QString styleSheet();

protected:
  virtual void focusInEvent(QFocusEvent *e) override;
  virtual void focusOutEvent(QFocusEvent *e) override;
  void updateBorderColor();

private:
  Q_DISABLE_COPY(APushButton)
  const QColor l_border_color = QColor(255, 255, 255);
  QColor m_border_color = l_border_color;
};

#endif // A_PUSHBUTTON_H
