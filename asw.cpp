#include "asw.h"

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
  setWindowTitle(basis->applicationName);
  setMinimumSize(minimalWidth, minimalHeight);
  layout()->setMargin(0);
  auto *centralWidget = new QWidget();
  centralWidget->setObjectName("cw");
  centralWidget->setStyleSheet(
      "#cw { background-color: qlineargradient(spread:pad, x1:0, y1:0, "
      "x2:1, y2:1, stop:0 rgba(191, 29, 160, 255), stop:1 rgba(6, 255, 255, "
      "255)); }");
  auto *layout = new QVBoxLayout();
  layout->setSpacing(0);
  layout->addWidget(display);
  layout->addWidget(line);
  centralWidget->setLayout(layout);
  setCentralWidget(centralWidget);
  setMenuBar(menubar);
  connector();
  applyingSettings();
}

/*!
 * Argument: QKeyEvent {*event} [transmitted automatically by Qt]
 * Handles keyboard shortcuts.
 */
void ASW::keyPressEvent(QKeyEvent *event) {
  if (event->modifiers() == (Qt::ControlModifier | Qt::ShiftModifier)) {
    if ((event->key() == Qt::Key_M))
      emit send("/mm");
    if (event->key() == Qt::Key_Less)
      emit send("/settings");
  }
  if (event->key() == Qt::Key_Return)
    event->modifiers() == Qt::ControlModifier ? line->lineEdit->insert("\n")
                                              : line->sendButton->click();
  if (event->modifiers() == Qt::ControlModifier) {
    if (event->key() == Qt::Key_H)
      menubar->setVisible(not menubar->isVisible());
    if (event->key() == Qt::Key_M)
      emit send("/sm");
    if (event->key() == Qt::Key_E)
      exportMessageHistory();
    if (event->key() == Qt::Key_I)
      importMessageHistory();
  }
  if (event->key() == Qt::Key_F11) {
    menubar->fullScreenAction->setChecked(
        not menubar->fullScreenAction->isChecked());
    // If the menu bar is hidden, it does not send signals.
    if (menubar->isHidden())
      fullScreenHandler();
  }
  if ((event->modifiers() == (Qt::ControlModifier | Qt::AltModifier)) &&
      (event->key() == Qt::Key_D))
    clear();
}

/*! Reads the settings from the file and applies. */
void ASW::applyingSettings() {
  // If settings file does not exist, sets default settings.
  if (not basis->exists() or basis->isIncorrect()) {
    resize(defaultWidth, defaultHeight);
    emit send("/first");
    saveWindowSettings();
    return;
  }
  resize(basis->read(basis->sizeSt).toSize());
  menubar->fullScreenAction->setChecked(
      basis->read(basis->isFullScreenSt).toBool());
  emit menubar->fullScreenAction->triggered();
  menubar->setVisible(not basis->read(basis->isMenuBarHiddenSt).toBool());
  menubar->emm->setChecked(
      basis->read(basis->isMonologueModeEnabledSt).toBool());
}

/*! Writes changes to window settings to a file. */
void ASW::saveWindowSettings() {
  if (basis->isUnaccessed())
    return;
  basis->write(basis->sizeSt, size());
  basis->write(basis->isMenuBarHiddenSt, menubar->isHidden());
  basis->write(basis->isFullScreenSt, isFullScreen());
  basis->write(basis->isNotFirstStartSt, true);
  basis->write(basis->isMonologueModeEnabledSt, menubar->emm->isChecked());
}

/*! Establishes communications for user interaction through the window. */
void ASW::connector() {
  // menubar
  connect(menubar->fullScreenAction, &QAction::triggered, this,
          &ASW::fullScreenHandler);
  connect(menubar, &AMenuBar::clearHistoryTriggered, this, &ASW::clear);
  connect(menubar, &AMenuBar::aboutTriggered, this,
          [this] { emit send("/about"); });
  connect(menubar, &AMenuBar::containersTriggered, this,
          [this] { emit send("/sm"); });
  connect(menubar, &AMenuBar::settingsTriggered, this,
          [this] { emit send("/settings"); });
  connect(menubar, &AMenuBar::exportTriggered, this,
          &ASW::exportMessageHistory);
  connect(menubar, &AMenuBar::importTriggered, this,
          &ASW::importMessageHistory);
  connect(menubar->emm, &QAction::triggered, this,
          [this] { emit send("/mm"); });
  // others
  connect(line->sendButton, &AButton::clicked, this, &ASW::userInputHandler);
  connect(this, &ASW::readyState, this, [this] { emit send(tr("Hello!")); });
  connect(this, &ASW::send, core, &Core::getUser);
  connect(core, &Core::show, display, &ADisplay::addMessage);
  connect(core, &Core::changeMenuBarMonologueCheckbox, menubar->emm,
          &QAction::setChecked);
}

/*! Shows a window in full screen or in normal mode. */
void ASW::fullScreenHandler() {
  menubar->fullScreenAction->isChecked() ? showFullScreen() : showNormal();
}

/*! Sends request to core. */
void ASW::userInputHandler() {
  // If the user sends a message, the display automatically scrolls to the end.
  display->setScrollEnabled(true);
  QString text = line->lineEdit->text();
  line->lineEdit->clear();
  emit send(text);
}

/*! Calls the dialog, asks for the filename {filename} and saves the message
 * history to it. */
void ASW::exportMessageHistory() {
  QString filename = QFileDialog::getSaveFileName(
      nullptr, tr("Save history"), nullptr, tr("JSON file") + "(*.json)");
  if (filename.isEmpty())
    return;
  historyProcessor->save(filename);
}

/*! Calls the dialog, asks for the filename {filename} and loads the message
 * history from it. */
void ASW::importMessageHistory() {
  if (QMessageBox::question(
          this, tr("Import message history?"),
          tr("Messages will be added to the beginning of the dialogue."),
          QMessageBox::Ok | QMessageBox::Cancel,
          QMessageBox::Ok) == QMessageBox::Ok) {
    QString filename = QFileDialog::getOpenFileName(
        nullptr, tr("Load history"), nullptr, tr("JSON file") + "(*.json)");
    if (filename.isEmpty())
      return;
    display->start();
    historyProcessor->load(filename);
  }
}

/*! Clears message history and display. */
void ASW::clear() {
  historyProcessor->clear();
  display->start();
}
