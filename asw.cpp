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

/*!
 * Argument: QKeyEvent {*event} [transmitted automatically by Qt]
 * Handles keyboard shortcuts.
 */
void ASW::keyPressEvent(QKeyEvent *event) {
  if (event->modifiers() == (Qt::ControlModifier | Qt::ShiftModifier)) {
    if ((event->key() == Qt::Key_M)) findASWCommand("/mm");
    if (event->key() == Qt::Key_Less) emit send("/settings");
  }
  if (event->key() == Qt::Key_Return)
    event->modifiers() == Qt::ControlModifier ? ln->lineEdit->insert("\n")
                                              : ln->sendButton->click();
  if (event->modifiers() == Qt::ControlModifier) {
    if (event->key() == Qt::Key_H) mb->setVisible(!mb->isVisible());
    if (event->key() == Qt::Key_M) emit send("/cm");
    if (event->key() == Qt::Key_E) exportMessageHistory();
    if (event->key() == Qt::Key_I) importMessageHistory();
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
    emit send("/first");
    return;
  }
  resize(cr->Meths->read(cr->Meths->sizeSt).toSize());
  mb->fullScreenAction->setChecked(
      cr->Meths->read(cr->Meths->isFullScreenSt).toBool());
  emit mb->fullScreenAction->triggered();
  mb->setVisible(!cr->Meths->read(cr->Meths->isMenuBarHiddenSt).toBool());
}

/*! Writes changes to window settings to a file. */
void ASW::saveWindowSettings() {
  if (cr->Meths->isUnaccessed()) return;
  cr->Meths->write(cr->Meths->sizeSt, size());
  cr->Meths->write(cr->Meths->isMenuBarHiddenSt, mb->isHidden());
  cr->Meths->write(cr->Meths->isFullScreenSt, isFullScreen());
  cr->Meths->write(cr->Meths->isNotFirstStartSt, true);
}

/*! Establishes communications for user interaction through the window. */
void ASW::connector() {
  // mb
  connect(mb->fullScreenAction, &QAction::triggered, this,
          &ASW::fullScreenHandler);
  connect(mb, &AMenuBar::clearHistoryTriggered, this, &ASW::clear);
  connect(mb, &AMenuBar::aboutTriggered, this, [this] { emit send("/about"); });
  connect(mb, &AMenuBar::containersTriggered, this,
          [this] { emit send("/cm"); });
  connect(mb, &AMenuBar::settingsTriggered, this,
          [this] { emit send("/settings"); });
  connect(mb, &AMenuBar::exportTriggered, this, &ASW::exportMessageHistory);
  connect(mb, &AMenuBar::importTriggered, this, &ASW::importMessageHistory);
  if (mb->emm != nullptr)
    connect(mb->emm, &QAction::triggered, this, [this] {
      cr->setMonologueEnabled(mb->emm->isChecked());
      emit send("/mm");
    });
  // others
  connect(ln->sendButton, &AButton::clicked, this, &ASW::userInputHandler);
  connect(this, &ASW::readyState, this, [this] { emit send(tr("Hello!")); });
  connect(this, &ASW::send, cr, &core::getUser);
  connect(cr, &core::show, this, &ASW::addMessage);
}

/*! Shows a window in full screen or in normal mode. */
void ASW::fullScreenHandler() {
  mb->fullScreenAction->isChecked() ? showFullScreen() : showNormal();
}

/*! Sends request to core {cr}. */
void ASW::userInputHandler() {
  // If the user sends a message, the display automatically scrolls to the end.
  d->setScrollEnabled(true);
  QString text = ln->lineEdit->text();
  ln->lineEdit->clear();
  if (!findASWCommand(text)) emit send(text);
}

/*! Calls the dialog, asks for the filename {fn} and saves the message history
 * to it. */
void ASW::exportMessageHistory() {
  QString fn = QFileDialog::getSaveFileName(
      nullptr, tr("Save history"), nullptr, tr("JSON file") + "(*.json)");
  if (fn.isEmpty()) return;
  cr->HistoryProcessor->save(fn);
}

/*! Calls the dialog, asks for the filename {fn} and loads the message history
 * from it. */
void ASW::importMessageHistory() {
  if (QMessageBox::question(
          this, tr("Import message history?"),
          tr("Messages will be added to the beginning of the dialogue."),
          QMessageBox::Ok | QMessageBox::Cancel,
          QMessageBox::Ok) == QMessageBox::Ok) {
    QString fn = QFileDialog::getOpenFileName(
        nullptr, tr("Load history"), nullptr, tr("JSON file") + "(*.json)");
    if (fn.isEmpty()) return;
    d->start();
    cr->HistoryProcessor->load(fn);
  }
}

/*! Clears message history and display. */
void ASW::clear() {
  cr->HistoryProcessor->clear();
  d->start();
}

/*!
 * Argument: QString {text} [user input].
 * Finds special commands in {text}.
 * Returns: found or not.
 */
bool ASW::findASWCommand(const QString &text) {
  // Enable monologue
  if (text == "/mm") {
    mb->emm->setChecked(!mb->emm->isChecked());
    emit mb->emm->triggered();
    return true;
  }
  return false;
}
