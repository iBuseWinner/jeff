#include "a_button.h"

AButton::AButton(const QString &t, QWidget *p) : QToolButton(p) {
  setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
  setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  setFocusPolicy(Qt::NoFocus);
  setStyleSheet(ss());
  setAutoRaise(true);
  setText(t);  
}

void AButton::mousePressEvent(QMouseEvent *e) {
  mbc = nbc;
  setStyleSheet(ss());
  QToolButton::mousePressEvent(e);
}

void AButton::mouseReleaseEvent(QMouseEvent *e) {
  mbc = lbc;
  setStyleSheet(ss());
  QToolButton::mouseReleaseEvent(e);
}
