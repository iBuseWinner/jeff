#include "jeff-ui.hpp"

/*! @brief The constructor.
 *  @details Layout scheme:
 *  <--------------->
 *  [    Display    ]
 *  {  Scroll area  }
 *  [     Line      ]
 *  {Text line}{Send}
 *  <--------------->  */
Jeff::Jeff() : QMainWindow() {
  setWindowIcon(QIcon(":/arts/icons/2000/icon.png"));
  setWindowTitle(tr("Jeff"));
  setMinimumSize(minimalWidth, minimalHeight);
  auto *centralWidget = new QWidget(this);
  centralWidget->setObjectName("cw");
  centralWidget->setStyleSheet(
    QString("#cw { background-color: %1; }")
    .arg(styling.light_theme ? styling.css_light_wbg : styling.css_dark_wbg)
  );
  centralWidget->setLayout(VLineLt::another()->margin(6)->spacing()->addw(display)->addlt(
    HLineLt::another()->spacing()->cmargins(5, 0, 5, 0)->addw(line)
  ));
  setCentralWidget(centralWidget);
  setMenuBar(menubar);
  connect_all();
  apply_settings();
}

/*! @brief Establishes communications for user interaction through the window. */
void Jeff::connect_all() {
  // menubar
  connect(&(menubar->full_screen_action), &QAction::triggered, this, &Jeff::full_screen_handler);
  connect(menubar, &MenuBar::clear_history_triggered, this, &Jeff::clear);
  connect(menubar, &MenuBar::export_triggered, this, &Jeff::export_message_history);
  connect(menubar, &MenuBar::import_triggered, this, &Jeff::import_message_history);
  connect(menubar, &MenuBar::exit_triggered, this, [this] { close(); });
  connect(menubar, &MenuBar::stop_scenario_triggered, this, [this] { emit send((*basis)[Basis::scenarioExitMsg].toString()); });
  // others
  connect(&(line->send_button), &Button::clicked, this, &Jeff::user_input_handler);
  connect(this, &Jeff::ready_state, core, &Core::start);
  connect(this, &Jeff::send, core, &Core::got_message_from_user);
  connect(core, &Core::show, display, &Display::add_message_by_md);
  connect(core, &Core::show_modal, display, &Display::add_message_with_widget);
  connect(core, &Core::show_status, display, &Display::update_status);
  connect(core, &Core::change_menubar_scenario_name, this, [this](QString name) {
    if (name.isEmpty())
      line->line_edit.setPlaceholderText(tr("Enter your message..."));
    else
      line->line_edit.setPlaceholderText(tr("Running: ") + name + " | " + tr("Enter your message..."));
  });
  connect(history_processor, &HProcessor::history_loaded, display, &Display::start_by);
}

/*! @brief Reads the settings from the file and applies them. */
void Jeff::apply_settings() {
  /*! If settings file does not exist, sets default settings. */
  if (not basis->exists() or not basis->correct()) {
    resize(defaultWidth, defaultHeight);
    emit send(Basis::first_start_cmd);
    basis->write(Basis::isGreetingsEnabledSt, true);
    basis->write(Basis::greetingsMsg, tr("Hello!"));
    basis->write(Basis::scenarioExitMsg, "//e");
    basis->write(Basis::serverPortSt, 8005);
    save_window_settings();
    return;
  }
  resize((*basis)[Basis::sizeSt].toSize());
  auto p = (*basis)[Basis::posSt].toPoint();
  if (not p.isNull()) move(p);
  menubar->full_screen_action.setChecked((*basis)[Basis::isFullScreenSt].toBool());
  emit menubar->full_screen_action.triggered();
  menubar->setVisible(not(*basis)[Basis::isMenuBarHiddenSt].toBool());
  menubar->enable_monologue_mode.setChecked((*basis)[Basis::isMonologueEnabledSt].toBool());
}

/*! @brief Handles keyboard shortcuts. */
void Jeff::keyPressEvent(QKeyEvent *event) {
  if (event->modifiers() == (Qt::ControlModifier | Qt::ShiftModifier)) {
    if (event->key() == Qt::Key_M) emit send(Basis::monologue_mode_cmd);
    if (event->key() == Qt::Key_Less) emit send(Basis::settings_cmd);
  }
  if (event->modifiers() == Qt::ControlModifier) {
    if (event->key() == Qt::Key_H) menubar->setVisible(not menubar->isVisible());
    if (event->key() == Qt::Key_M) emit send(Basis::source_manager_cmd);
    if (event->key() == Qt::Key_E) export_message_history();
    if (event->key() == Qt::Key_I) import_message_history();
  }
  if (event->key() == Qt::Key_F11) {
    menubar->full_screen_action.setChecked(not menubar->full_screen_action.isChecked());
    /*! If the menu bar is hidden, it does not send signals. */
    if (menubar->isHidden()) full_screen_handler();
  }
  if ((event->modifiers() == (Qt::ControlModifier | Qt::AltModifier)) and (event->key() == Qt::Key_D))
    clear();
}

/*! @brief Closes the app. */
void Jeff::closeEvent(QCloseEvent *event) {
  save_window_settings();
  hide();
  core->em->shutdown_extensions();
  event->accept();
}

/*! @brief Writes changes to window settings to a file. */
void Jeff::save_window_settings() {
  if (not basis->accessible()) return;
  basis->write(Basis::posSt, pos());
  basis->write(Basis::sizeSt, size());
  basis->write(Basis::isNotFirstStartSt, true);
  basis->write(Basis::isFullScreenSt, isFullScreen());
  basis->write(Basis::isMenuBarHiddenSt, menubar->isHidden());
  basis->write(Basis::isMonologueEnabledSt, menubar->enable_monologue_mode.isChecked());
}

/*! @brief Shows a window in full screen or in normal mode. */
void Jeff::full_screen_handler() {
  menubar->full_screen_action.isChecked() ? showFullScreen() : showNormal();
}

/*! @brief Sends a request to Core. */
void Jeff::user_input_handler() {
  // If user sends a message, the display automatically scrolls to the end.
  display->set_scroll_enabled(true);
  QString text = line->line_edit.text().trimmed();
  line->line_edit.clear();
  emit send(text);
}

/*! @brief Calls the dialog, asks for @a filename and saves the message history to it. */
void Jeff::export_message_history() {
  QString filename = QFileDialog::getSaveFileName(
    nullptr, tr("Save history"), nullptr, tr("Jeff's history file") + "(*.history.json)");
  if (filename.isEmpty()) return;
  history_processor->save(filename);
}

/*! @brief Calls the dialog, asks for @a filename and loads the message history from it. */
void Jeff::import_message_history() {
  if (QMessageBox::question(
    this, tr("Import message history?"),
    tr("Messages will be added to the beginning of the dialogue."),
    QMessageBox::Ok | QMessageBox::Cancel,
    QMessageBox::Ok) == QMessageBox::Ok
  ) {
    QString filename = QFileDialog::getOpenFileName(
      nullptr, tr("Load history"), nullptr, tr("Jeff's history file") + "(*.history.json)");
    if (filename.isEmpty()) return;
    history_processor->load(filename);
  }
}

/*! @brief Clears message history and display. */
void Jeff::clear() { history_processor->clear(); display->start(); }
