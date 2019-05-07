#include "a_message.h"

AMessage::AMessage() {
  QHBoxLayout *l = new QHBoxLayout();
  l->setContentsMargins(0, 0, 0, bm);
  l->setSpacing(0);
  setLayout(l);
}

AMessage::AMessage(message _daemon) {
  QHBoxLayout *l = new QHBoxLayout();
  l->setContentsMargins(0, 0, 0, bm);
  l->setSpacing(0);
  setLayout(l);
  setDaemon(_daemon);
}

void AMessage::setDaemon(message _daemon) {
  if (daemon.dt != "") return;
  daemon = _daemon;
  setAuthor(daemon.aType);
  setMessageType(daemon.cType);
  // setTheme(daemon.tType);
}

void AMessage::setWidget(QWidget *_w) {
  if (w != nullptr) return;
  w = _w;
  connect(w, &QWidget::destroyed, this, &AMessage::close);
  el->addWidget(w);
}

void AMessage::setAuthor(A _a) {
  switch (_a) {
    case 1:
      setupASW();
      break;
    case 2:
      setupUser();
      break;
    default:;
  }
}

void AMessage::setMessageType(C _ct) {
  switch (_ct) {
    case 1:
      setupText(daemon.cn);
      break;
    case 2:
      setupMarkdown(daemon.cn);
      break;
    // case 3:
    //  setupPicture(daemon.cn);
    //  break;
    // case 4:
    //  setupFile(daemon.cn);
    //  break;
    case 5:
      setupWarning(daemon.cn);
      break;
    case 6:
      setupError(daemon.cn);
      break;
    case 7:
      prepareSetupWidget();
      return;
    default:;
  }
  connect(w, &QWidget::destroyed, this, &AMessage::close);
  alignTextToWindowWidth();
}

// void AMessage::setTheme(T _t) {}

void AMessage::setupASW() {
  QPair<QSpacerItem *, ABoard *> ws = mk();
  layout()->addWidget(ws.second);
  layout()->addItem(ws.first);
}

void AMessage::setupUser() {
  QPair<QSpacerItem *, ABoard *> ws = mk();
  layout()->addItem(ws.first);
  layout()->addWidget(ws.second);
}

void AMessage::setupText(const QString &_cn) {
  auto *tl = new QLabel(_cn, this);
  w = tl;
  tl->setObjectName("text");
  tl->setTextFormat(Qt::PlainText);
  tl->setTextInteractionFlags(Qt::TextSelectableByMouse |
                              Qt::TextSelectableByKeyboard);
  tl->setFocusPolicy(Qt::NoFocus);
  el->addWidget(w);
}

void AMessage::setupMarkdown(const QString &_cn) {
  setupText(_cn);
  auto *tl = static_cast<QLabel *>(w);  // test
  tl->setTextFormat(Qt::RichText);
  tl->setText(tl->text().replace("\n", "<br>"));
}

// void AMessage::setupPicture(QString path) {}

// void AMessage::setupFile(QString path) {}

void AMessage::setupWarning(const QString &_cn) {
  setupText(QString("Warning: " + _cn));
}

void AMessage::setupError(const QString &_cn) {
  setupText(QString("Error: " + _cn));
}

void AMessage::prepareSetupWidget() {
  el->parentWidget()->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
}

void AMessage::alignTextToWindowWidth() {
  auto *_tl = static_cast<QLabel *>(w);
  if (_tl == nullptr) return;
  QTextDocument td;
  if (daemon.cType == C::Markdown)
    td.setHtml(_tl->text());
  else if (daemon.cType == C::Text)
    td.setPlainText(_tl->text());
  else
    return;
  if (td.idealWidth() < mmw)
    _tl->setWordWrap(false);
  else {
    _tl->setWordWrap(true);
    _tl->setFixedWidth(mmw);
  }
}

QPair<QSpacerItem *, ABoard *> AMessage::mk() {
  auto *s = new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding,
                            QSizePolicy::Minimum);
  auto *b = new ABoard(this);
  el = new QGridLayout();
  el->setSpacing(0);
  el->setMargin(6);
  b->setLayout(el);
  return QPair<QSpacerItem *, ABoard *>(s, b);
}
