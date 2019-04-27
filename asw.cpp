#include "asw.h"

ASW::ASW(QWidget *p) : QMainWindow(p) {
  setWindowIcon(QIcon(":/arts/icons/500/icon.png"));
  // Removes interface padding, making it smaller...
  layout()->setMargin(0);
  layout()->setSpacing(0);
  // Creates central widget and connected widgets...
  QWidget *cw = new QWidget();
  cw->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
  cw->setObjectName("asw_bg");
  cw->setStyleSheet(
      "#asw_bg {"
      "background-image: url(:/arts/background.jpg);"
      "background-position: center center;"
      "background-repeat:  no-repeat;"
      "background-attachment: fixed;"
      "}");
  QLayout *el = new QVBoxLayout();
  el->setSpacing(0);
  d = new ADisplay(this);
  l = new ALine(this);
  mb = new AMenuBar(l, this);
  // Appends widgets and output...
  el->addWidget(d);
  el->addWidget(l);
  setMenuBar(mb);
  cw->setLayout(el);
  setCentralWidget(cw);
  QSqlDatabase::addDatabase("QSQLITE");
  connector();
  applyingSettings();
  saveSettings();
  emit readyState();
}

void ASW::applyingSettings() {
  setWindowTitle(st->application);
  // It's not allowed to reduce the size of the
  // window is less than these values...
  setMinimumSize(mw, mh);
  resize(currw, currh);
  // Restores window settings...
  if (st->read(sizeSt).toSize() != QSize(-1, -1))
    resize(st->read(sizeSt).toSize());
  if (st->read(isFullscreenSt).toBool()) {
    showFullScreen();
    mb->fsa->setChecked(true);
    // If ASW starts first time...
  }
  if (!st->read(isNotFirstStartSt).toBool())
    fsStarter();
  else
    saveSettings();
  mb->setVisible(!st->read(isMenubarHiddenSt).toBool());
}

void ASW::connector() {
  // Connects signals with slots.
  connect(l->s, &APushButton::clicked, this, &ASW::userSendsMessage);
  connect(mb, &AMenuBar::fullscreenModeChanged, this, &ASW::fullScreenHandler);
  connect(mb, &AMenuBar::clearScreenTriggered, this, &ASW::clearScreen);
  connect(mb, &AMenuBar::aboutTriggered, this, &ASW::aboutStarter);
  connect(mb, &AMenuBar::contManTriggered, this, &ASW::cmStarter);
  connect(this, &ASW::readyState, this, &ASW::greeting);
}

void ASW::saveSettings() {
  st->write(sizeSt, this->size());
  st->write(isMenubarHiddenSt, mb->isHidden());
  st->write(isFullscreenSt, this->isFullScreen());
  st->write(isNotFirstStartSt, true);
}

void ASW::greeting() { addMessage(AMessage::ASW, "hello!"); }

void ASW::addMessage(AMessage::A a, const QString &_t) {
  if (_t.trimmed() == "") return;
  AMessage *msg = nullptr;
  if (a == AMessage::User) {
    l->tl->clear();
    d->se = true;
    msg = new AMessage(this);
    msg->setAuthor(a);
    msg->setMessageType(AMessage::HTML, _t);
  } else {
    msg = nm->get(_t, a, this);
    if (msg == nullptr) return;
    if ((msg->returnMessageType() != AMessage::Widget) and
        (msg->returnText().trimmed() == "")) {
      delete msg;
      return;
    }
  }
  ms.append(msg);
  d->l->addWidget(msg);
  // Responses to user expression...
  if (a == AMessage::User) addMessage(AMessage::ASW, _t);
}

void ASW::userSendsMessage() { addMessage(AMessage::User, l->tl->text()); }

void ASW::keyPressEvent(QKeyEvent *event) {
  if ((event->modifiers() == Qt::ControlModifier) &&
      (event->key() == Qt::Key_H))
    mb->setVisible(!mb->isVisible());
  if (!mb->isVisible()) {
    if (event->key() == Qt::Key_F11) {
      mb->fsa->setChecked(!mb->fsa->isChecked());
      fullScreenHandler();
    }
    if ((event->modifiers() == (Qt::ControlModifier | Qt::AltModifier)) &&
        (event->key() == Qt::Key_D))
      clearScreen();
  }
}

void ASW::fullScreenHandler() {
  if (mb->fsa->isChecked())
    showFullScreen();
  else
    showNormal();
}

void ASW::clearScreen() {
  d->start();
  ms.clear();
}

void ASW::fsStarter() { addMessage(AMessage::User, "/first"); }

void ASW::aboutStarter() { addMessage(AMessage::User, "/about"); }

void ASW::cmStarter() { addMessage(AMessage::User, "/cm"); }

void ASW::helpStarter() { addMessage(AMessage::User, "/help"); }
