#include "asw.h"

ASW::ASW() : QMainWindow() {
  setWindowIcon(QIcon(":/arts/icons/500/icon.png"));
  setMinimumSize(mw, mh);
  layout()->setMargin(0);
  layout()->setSpacing(0);
  auto *cw = new QWidget();
  cw->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
  cw->setObjectName("asw_bg");
  cw->setStyleSheet(
      "#asw_bg { background-color: qlineargradient(spread:pad, x1:0, y1:0, "
      "x2:1, y2:1, stop:0 rgba(191, 29, 160, 255), stop:1 rgba(6, 255, 255, "
      "255)); }");
  auto *el = new QVBoxLayout();
  el->setSpacing(0);
  el->addWidget(d);
  el->addWidget(l);
  setMenuBar(mb);
  cw->setLayout(el);
  setCentralWidget(cw);
  connector();
  applyingSettings();
  saveSettings();
  emit readyState();
}

void ASW::greeting() { emit send("Hello!"); }

void ASW::keyPressEvent(QKeyEvent *event) {
  if ((event->modifiers() == Qt::ControlModifier) &&
      (event->key() == Qt::Key_H))
    mb->setVisible(!mb->isVisible());
  if (!mb->isVisible()) {
    if (event->key() == Qt::Key_F11) {
      mb->fsa->setChecked(!mb->fsa->isChecked());
      fsh();
    }
    if ((event->modifiers() == (Qt::ControlModifier | Qt::AltModifier)) &&
        (event->key() == Qt::Key_D))
      clearScreen();
  }
}

void ASW::applyingSettings() {
  setWindowTitle(cr->st->a);
  if (cr->st->read(sizeSt).toSize() != QSize(-1, -1))
    resize(cr->st->read(sizeSt).toSize());
  else
    resize(stdw, stdh);
  if (cr->st->read(isFullscreenSt).toBool()) {
    showFullScreen();
    mb->fsa->setChecked(true);
  }
  if (!cr->st->read(isNotFirstStartSt).toBool()) fsStarter();
  mb->setVisible(!cr->st->read(isMenubarHiddenSt).toBool());
}

void ASW::saveSettings() {
  cr->st->write(sizeSt, this->size());
  cr->st->write(isMenubarHiddenSt, mb->isHidden());
  cr->st->write(isFullscreenSt, this->isFullScreen());
  cr->st->write(isNotFirstStartSt, true);
}

void ASW::connector() {
  connect(l->s, &AButton::clicked, this, &ASW::userInput);
  connect(mb, &AMenuBar::fullscreenModeChanged, this, &ASW::fsh);
  connect(mb, &AMenuBar::clearHistoryTriggered, this, &ASW::clearScreen);
  connect(mb, &AMenuBar::aboutTriggered, this, &ASW::aboutStarter);
  connect(mb, &AMenuBar::containersTriggered, this, &ASW::cmStarter);
  connect(this, &ASW::readyState, this, &ASW::greeting);
  connect(this, &ASW::send, cr, &core::getUser);
  connect(cr, &core::show, this, &ASW::addMessage);
  connect(mb, &AMenuBar::exportTriggered, cr->hp,
          &history_processor::exportVisibleHistory);
}

void ASW::fsh() {
  if (mb->fsa->isChecked())
    showFullScreen();
  else
    showNormal();
}

void ASW::userInput() {
  d->se = true;
  emit send(l->tl->text());
  l->tl->clear();
}

void ASW::clearScreen() {
  d->start();
  cr->hp->clearVisibleHistory();
}

void ASW::fsStarter() { emit send("/first"); }

void ASW::aboutStarter() { emit send("/about"); }

void ASW::cmStarter() { emit send("/cm"); }

// void ASW::helpStarter() { emit send("/help"); }

void ASW::addMessage(AMessage *msg) {
  cr->hp->addVisibleMessage(msg);
  d->l->addWidget(msg);
}
