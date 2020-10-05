#include "asw.h"

/*!
 * @fn ASW::ASW
 * @brief The constructor.
 * @details Layout scheme:
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
  auto *centralWidget = new QWidget(this);
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
  connect_all();
  apply_settings();
}

/*!
 * @fn ASW::keyPressEvent
 * @brief Handles keyboard shortcuts.
 * @param[in,out] event key event
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
      export_message_history();
    if (event->key() == Qt::Key_I)
      import_message_history();
  }
  if (event->key() == Qt::Key_F11) {
    menubar->fullScreenAction->setChecked(
        not menubar->fullScreenAction->isChecked());
    /*! If the menu bar is hidden, it does not send signals. */
    if (menubar->isHidden())
      full_screen_handler();
  }
  if ((event->modifiers() == (Qt::ControlModifier | Qt::AltModifier)) &&
      (event->key() == Qt::Key_D))
    clear();
}

/*!
 * @fn ASW::apply_settings
 * @brief Reads the settings from the file and applies them.
 */
void ASW::apply_settings() {
  /*! If settings file does not exist, sets default settings. */
  if (not basis->exists() or not basis->correct()) {
    resize(defaultWidth, defaultHeight);
    emit send("/first");
    save_window_settings();
    return;
  }
  resize((*basis)[basis->sizeSt].toSize());
  menubar->fullScreenAction->setChecked(
      (*basis)[basis->isFullScreenSt].toBool());
  emit menubar->fullScreenAction->triggered();
  menubar->setVisible(not(*basis)[basis->isMenuBarHiddenSt].toBool());
  menubar->emm->setChecked((*basis)[basis->isMonologueModeEnabledSt].toBool());
}

/*!
 * @fn ASW::save_window_settings
 * @brief Writes changes to window settings to a file.
 */
void ASW::save_window_settings() {
  if (not basis->accessible())
    return;
  basis->write(basis->sizeSt, size());
  basis->write(basis->isMenuBarHiddenSt, menubar->isHidden());
  basis->write(basis->isFullScreenSt, isFullScreen());
  basis->write(basis->isNotFirstStartSt, true);
  basis->write(basis->isMonologueModeEnabledSt, menubar->emm->isChecked());
}

/*!
 * @fn ASW::connect_all
 * @brief Establishes communications for user interaction through the window.
 */
void ASW::connect_all() {
  // menubar
  connect(menubar->fullScreenAction, &QAction::triggered, this,
          &ASW::full_screen_handler);
  connect(menubar, &AMenuBar::clearHistoryTriggered, this, &ASW::clear);
  connect(menubar, &AMenuBar::aboutTriggered, this,
          [this] { emit send("/about"); });
  connect(menubar, &AMenuBar::sourcesTriggered, this,
          [this] { emit send("/sm"); });
  connect(menubar, &AMenuBar::settingsTriggered, this,
          [this] { emit send("/settings"); });
  connect(menubar, &AMenuBar::exportTriggered, this,
          &ASW::export_message_history);
  connect(menubar, &AMenuBar::importTriggered, this,
          &ASW::import_message_history);
  connect(menubar->emm, &QAction::triggered, this,
          [this] { emit send("/mm"); });
  // others
  connect(line->sendButton, &AButton::clicked, this, &ASW::user_input_handler);
  connect(this, &ASW::ready_state, this, [this] { emit send(tr("Hello!")); });
  connect(this, &ASW::send, core, &Core::got_message_from_user);
  connect(core, &Core::show, display, &ADisplay::addMessage);
  connect(core, &Core::changeMenuBarMonologueCheckbox, menubar->emm,
          &QAction::setChecked);
}

/*!
 * @fn ASW::full_screen_handler
 * @brief Shows a window in full screen or in normal mode.
 */
void ASW::full_screen_handler() {
  menubar->fullScreenAction->isChecked() ? showFullScreen() : showNormal();
}

/*!
 * @fn ASW::user_input_handler
 * @brief Sends a request to Core.
 * @sa Core
 */
void ASW::user_input_handler() {
  // If the user sends a message, the display automatically scrolls to the end.
  display->setScrollEnabled(true);
  QString text = line->lineEdit->text();
  line->lineEdit->clear();
  emit send(text);
}

/*!
 * @fn ASW::export_message_history
 * @brief Calls the dialog, asks for @a filename and saves the message history
 * to it.
 */
void ASW::export_message_history() {
  QString filename = QFileDialog::getSaveFileName(
      nullptr, tr("Save history"), nullptr, tr("JSON file") + "(*.json)");
  if (filename.isEmpty())
    return;
  history_processor->save(filename);
}

/*!
 * @fn ASW::import_message_history
 * @brief Calls the dialog, asks for @a filename and loads the message history
 * from it.
 */
void ASW::import_message_history() {
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
    history_processor->load(filename);
  }
}

/*!
 * @fn ASW::clear
 * @brief Clears message history and display.
 */
void ASW::clear() {
  history_processor->clear();
  display->start();
}
