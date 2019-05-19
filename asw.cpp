#include "asw.h"

/*
 * All short named objects and their explanations:
 * {mw} <- minimal width
 * {mh} <- minimal height
 * {stdw} <- standard width
 * {stdh} <- standard height
 * {cw} <- central widget
 * {lt} <- layout
 * {d} <- display
 * {ln} <- line
 * {mb} <- menubar
 * {cr} <- core
 * {fn} <- filename
 * {msg} <- message
 */

/*!
 * Constructs and prepares the ASW window.
 *
 * Layout scheme:
 * <--------------->
 * [    Display    ]
 * {  Scroll area  }
 * [     Line      ]
 * {Text line}{Send}
 * <--------------->
 */
ASW::ASW() : QMainWindow() {
  setWindowIcon(QIcon(":/arts/icons/500/icon.png"));
  setWindowTitle(cr->Meths->applicationName);
  setMinimumSize(mw, mh);
  layout()->setMargin(0);
  auto *cw = new QWidget();
  cw->setObjectName("cw");
  cw->setStyleSheet(
      "#cw { background-color: qlineargradient(spread:pad, x1:0, y1:0, "
      "x2:1, y2:1, stop:0 rgba(191, 29, 160, 255), stop:1 rgba(6, 255, 255, "
      "255)); }");
  auto *lt = new QVBoxLayout();
  lt->setSpacing(0);
  lt->addWidget(d);
  lt->addWidget(ln);
  cw->setLayout(lt);
  setCentralWidget(cw);
  setMenuBar(mb);
  connector();
  applyingSettings();
  emit readyState();
}

/*! Sends the initial message or greeting. */
void ASW::greeting() { emit send("Hello!"); }

/*!
 * Argument: QKeyEvent {*event} [transmitted automatically by Qt]
 * Handles keyboard shortcuts.
 */
void ASW::keyPressEvent(QKeyEvent *event) {
  if (event->modifiers() == Qt::ControlModifier) {
    if (event->key() == Qt::Key_H) mb->setVisible(!mb->isVisible());
    if (event->key() == Qt::Key_M) containerManager();
    if (event->key() == Qt::Key_S) exportMessageHistory();
  }
  if (event->key() == Qt::Key_F11) {
    mb->fullScreenAction->setChecked(!mb->fullScreenAction->isChecked());
    // If the menu bar is hidden, it does not send signals.
    if (mb->isHidden()) fullScreenHandler();
  }
  if ((event->modifiers() == (Qt::ControlModifier | Qt::AltModifier)) &&
      (event->key() == Qt::Key_D))
    clear();
}

/*! Reads the settings from the file and applies. */
void ASW::applyingSettings() {
  // If settings file does not exist, sets default settings.
  if ((!cr->Meths->exists()) || (cr->Meths->isIncorrect())) {
    resize(stdw, stdh);
    firstStart();
    return;
  }
  resize(cr->Meths->read(sizeSt).toSize());
  mb->fullScreenAction->setChecked(cr->Meths->read(isFullScreenSt).toBool());
  mb->setVisible(!cr->Meths->read(isMenuBarHiddenSt).toBool());
}

/*! Writes changes to window settings to a file. */
void ASW::saveWindowSettings() {
  if (cr->Meths->isUnaccessed()) return;
  cr->Meths->write(sizeSt, size());
  cr->Meths->write(isMenuBarHiddenSt, mb->isHidden());
  cr->Meths->write(isFullScreenSt, isFullScreen());
  cr->Meths->write(isNotFirstStartSt, true);
}

/*! Establishes communications for user interaction through the window. */
void ASW::connector() {
  connect(ln->sendButton, &AButton::clicked, this, &ASW::userInputHandler);
  connect(mb, &AMenuBar::fullscreenModeChanged, this, &ASW::fullScreenHandler);
  connect(mb, &AMenuBar::clearHistoryTriggered, this, &ASW::clear);
  connect(mb, &AMenuBar::aboutTriggered, this, &ASW::about);
  connect(mb, &AMenuBar::containersTriggered, this, &ASW::containerManager);
  connect(this, &ASW::readyState, this, &ASW::greeting);
  connect(this, &ASW::send, cr, &core::getUser);
  connect(cr, &core::show, this, &ASW::addMessage);
  connect(mb, &AMenuBar::exportTriggered, this, &ASW::exportMessageHistory);
  connect(mb, &AMenuBar::importTriggered, this, &ASW::importMessageHistory);
}

/*! Shows a window in full screen or in normal mode. */
void ASW::fullScreenHandler() {
  mb->fullScreenAction->isChecked() ? showFullScreen() : showNormal();
}

/*! Sends request to core {cr}. */
void ASW::userInputHandler() {
  // If the user sends a message, the display automatically scrolls to the end.
  d->setScrollEnabled(true);
  emit send(ln->lineEdit->text());
  ln->lineEdit->clear();
}

/*! Calls the dialog, asks for the filename {fn} and saves the message history
 * to it. */
void ASW::exportMessageHistory() {
  QString fn = QFileDialog::getSaveFileName(
      nullptr, tr("Save history"), nullptr, tr("JSON file") + "(*.json)");
  if (fn == "") return;
  cr->HistoryProcessor->save(fn);
}

/*! */
void ASW::importMessageHistory() {
  QString fn = QFileDialog::getOpenFileName(
      nullptr, tr("Load history"), nullptr, tr("JSON file") + "(*.json)");
  if (fn == "") return;
  d->start();
  cr->HistoryProcessor->load(fn);
}

/*! Clears message history and display. */
void ASW::clear() {
  cr->HistoryProcessor->clear();
  d->start();
}
