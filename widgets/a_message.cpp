#include "a_message.h"

AMessage::AMessage(QWidget *parent) : QWidget(parent) {
  // Creates a message.
  el->setContentsMargins(0, 0, 0, bm);
  el->setSpacing(0);
  setLayout(el);
}

void AMessage::setAuthor(A _a) {
  if (a != A::undefA) return;
  a = _a;
  switch (a) {
    case 1:
      createASWLayout();
      break;
    case 2:
      createUserLayout();
      break;
    default:;
  }
}

void AMessage::createASWLayout() {
  auto *spacer = new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding,
                                 QSizePolicy::Minimum);
  el->addWidget(b);  // content is on the left
  el->addItem(spacer);
  update();
}

void AMessage::createUserLayout() {
  auto *spacer = new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding,
                                 QSizePolicy::Minimum);
  el->addItem(spacer);
  el->addWidget(b);  // content is on the right
  update();
}

void AMessage::setMessageType(CT _ct, const QString &_cn) {
  if (ct != CT::undefCT) return;
  ct = _ct;
  switch (ct) {
    case 1:
      setupPlainMessage(_cn);
      break;
    case 2:
      setupHTMLMessage(_cn);
      break;
    // case 4:
    //   setupMessage_pict(_cn);
    //   break;
    // case 5:
    //   setupMessage_file(_cn);
    //   break;
    default:;
  }
}

void AMessage::setMessageType(CT _ct, QWidget *_cn) {
  if (ct != CT::undefCT) return;
  ct = _ct;
  setupWidgetMessage(_cn);
}

void AMessage::setupPlainMessage(const QString &_cn) {
  cn = _cn;
  tl = new QLabel(_cn, this);
  tl->setTextFormat(Qt::PlainText);
  auto *bl = new QGridLayout();
  bl->addWidget(tl);
  b->setLayout(bl);
  alignTextToWindowWidth();
}

void AMessage::setupHTMLMessage(const QString &_cn) {
  setupPlainMessage(_cn);
  tl->setTextFormat(Qt::RichText);
}

void AMessage::setupWidgetMessage(QWidget *_cn) {
  auto *bl = new QGridLayout();
  bl->addWidget(_cn);
  b->setLayout(bl);
  b->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
}

// void AMessage::setupMessage_pict(QString path) {}

// void AMessage::setupMessage_file(QString path) {}

QString AMessage::returnText() {
  if (returnMessageType() != AMessage::Widget) return cn;
  return "";
}

void AMessage::alignTextToWindowWidth() {
  if ((ct != CT::Text) && (ct != CT::HTML)) return;
  if (tl->fontMetrics().width(cn) < mmw) {
    tl->setWordWrap(false);
    tl->setFixedWidth(tl->fontMetrics().width(cn));
  } else if (tl->fontMetrics().width(cn) > mmw) {
    tl->setWordWrap(true);
    tl->setFixedWidth(mmw);
  }
}
