#ifndef AKIWAKE_PUSHBUTTON_H
#define AKIWAKE_PUSHBUTTON_H

#include <QAbstractAnimation>
#include <QAction>
#include <QFocusEvent>
#include <QFontDatabase>
#include <QObject>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QStyle>

class AkiwakePushButton : public QPushButton {
  Q_OBJECT
  Q_PROPERTY(
      QColor border_color MEMBER m_border_color NOTIFY borderColorChanged)
  Q_PROPERTY(QString theme MEMBER m_theme)

public:
  explicit AkiwakePushButton(QString Text, QWidget *parent = nullptr);
  ~AkiwakePushButton() override;
  QPropertyAnimation *animation = new QPropertyAnimation();
  QString styleSheet();

signals:
  void borderColorChanged();

protected:
  virtual void focusInEvent(QFocusEvent *e) override;
  virtual void focusOutEvent(QFocusEvent *e) override;
  void setBorderColor();

private:
  Q_DISABLE_COPY(AkiwakePushButton)
  const QColor l_border_color = QColor(255, 255, 255);
  const QString l_theme = "light";
  const QColor d_border_color = QColor(0, 0, 0);
  const QString d_theme = "dark";
  QColor m_border_color = l_border_color;
  QString m_theme = l_theme;
};

#endif // AKIWAKE_PUSHBUTTON_H
