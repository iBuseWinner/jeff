#include "menubar.hpp"

/*! @brief The constructor. */
MenuBar::MenuBar(Core *_core, Line *line, QWidget *parent) : QMenuBar(parent), core(_core) {
  constexpr const char space = ' ';
  // File
  file_menu.setTitle(tr("File"));
  source_manager_action.setText(tr("Source manager") + space + Basis::source_manager_cmd);
  phrase_editor_action.setText(tr("Phrase editor") + space + Basis::phrase_editor_cmd);
  script_manager_action.setText(tr("Scripts' manager") + space + Basis::source_manager_cmd);
  export_history_action.setText(tr("Export message history"));
  import_history_action.setText(tr("Import message history"));
  enable_monologue_mode.setText(tr("Enable monologue mode") + space + Basis::monologue_mode_cmd);
  enable_monologue_mode.setCheckable(true);
  exit_action.setText(tr("&Exit"));
  source_manager_action.setShortcut(Qt::CTRL | static_cast<int>(Qt::Key_M));
  phrase_editor_action.setShortcut(Qt::CTRL | static_cast<int>(Qt::Key_P));
  export_history_action.setShortcut(Qt::CTRL | static_cast<int>(Qt::Key_E));
  import_history_action.setShortcut(Qt::CTRL | static_cast<int>(Qt::Key_I));
  enable_monologue_mode.setShortcut(Qt::CTRL | Qt::SHIFT | Qt::Key_M);
  exit_action.setShortcut(Qt::ALT | static_cast<int>(Qt::Key_F4));
  source_manager_action.setIcon(
    QIcon::fromTheme("network-server-database", QIcon(":/arts/icons/16/database-manager.svg")));
  phrase_editor_action.setIcon(
    QIcon::fromTheme("node-add", QIcon(":/arts/icons/16/format-add-node.svg")));
  export_history_action.setIcon(
    QIcon::fromTheme("document-export", QIcon(":/arts/icons/16/document-export.svg")));
  import_history_action.setIcon(
    QIcon::fromTheme("document-import", QIcon(":/arts/icons/16/document-import.svg")));
  enable_monologue_mode.setIcon(
    QIcon::fromTheme("user-group-properties", QIcon(":/arts/icons/16/monologue.svg")));
  exit_action.setIcon(
    QIcon::fromTheme("application-exit", QIcon(":/arts/icons/16/application-exit.svg")));
  file_menu.addAction(&source_manager_action);
  file_menu.addAction(&phrase_editor_action);
  file_menu.addAction(&enable_monologue_mode);
  file_menu.addSeparator();
  file_menu.addAction(&export_history_action);
  file_menu.addAction(&import_history_action);
  file_menu.addSeparator();
  file_menu.addAction(&exit_action);
  connect(&source_manager_action, &QAction::triggered, this, [this] {
    core->got_message_from_user(Basis::source_manager_cmd);
  });
  connect(&phrase_editor_action, &QAction::triggered, this, [this] {
    core->got_message_from_user(Basis::phrase_editor_cmd);
  });
  connect(&export_history_action, &QAction::triggered, this, [this] { emit export_triggered(); });
  connect(&import_history_action, &QAction::triggered, this, [this] { emit import_triggered(); });
  connect(&enable_monologue_mode, &QAction::triggered, this, [this] {
    core->got_message_from_user(Basis::monologue_mode_cmd);
  });
  connect(&enable_monologue_mode, &QAction::toggled, this, [this] {
    if (enable_monologue_mode.isChecked())
      enable_monologue_mode.setText(tr("Disable monologue mode") + space + Basis::monologue_mode_cmd);
    else
      enable_monologue_mode.setText(tr("Enable monologue mode") + space + Basis::monologue_mode_cmd);
  });
  connect(core, &Core::change_menubar_mmode, &enable_monologue_mode, &QAction::setChecked);
  connect(&exit_action, &QAction::triggered, this, [this] { emit exit_triggered(); });
  addMenu(&file_menu);
  // Edit
  edit_menu.setTitle(tr("Edit"));
  clear_history_action.setText(tr("Clear message history"));
  delete_text_action.setText(tr("Delete"));
  cut_text_action.setText(tr("Cut"));
  copy_text_action.setText(tr("Copy"));
  paste_text_action.setText(tr("Paste"));
  select_all_text_action.setText(tr("Select all"));
  clear_history_action.setShortcut(Qt::CTRL | Qt::ALT | Qt::Key_D);
  delete_text_action.setShortcut(Qt::Key_Backspace);
  cut_text_action.setShortcuts(QKeySequence::Cut);
  copy_text_action.setShortcuts(QKeySequence::Copy);
  paste_text_action.setShortcuts(QKeySequence::Paste);
  select_all_text_action.setShortcuts(QKeySequence::SelectAll);
  clear_history_action.setIcon(
    QIcon::fromTheme("edit-clear-history", QIcon(":/arts/icons/16/clear-history.svg")));
  delete_text_action.setIcon(
    QIcon::fromTheme("edit-clear", QIcon(":/arts/icons/16/clear.svg")));
  cut_text_action.setIcon(
    QIcon::fromTheme("edit-cut", QIcon(":/arts/icons/16/cut.svg")));
  copy_text_action.setIcon(
    QIcon::fromTheme("edit-copy", QIcon(":/arts/icons/16/copy.svg")));
  paste_text_action.setIcon(
    QIcon::fromTheme("edit-paste", QIcon(":/arts/icons/16/paste.svg")));
  select_all_text_action.setIcon(
    QIcon::fromTheme("edit-select-all", QIcon(":/arts/icons/16/select-all.svg")));
  edit_menu.addAction(&clear_history_action);
  edit_menu.addSeparator();
  edit_menu.addAction(&delete_text_action);
  edit_menu.addAction(&cut_text_action);
  edit_menu.addAction(&copy_text_action);
  edit_menu.addAction(&paste_text_action);
  edit_menu.addSeparator();
  edit_menu.addAction(&select_all_text_action);
  connect(&clear_history_action, &QAction::triggered, this, [this] { emit clear_history_triggered(); });
  connect(&delete_text_action, &QAction::triggered, &(line->line_edit), &TextEdit::backspace);
  connect(&cut_text_action, &QAction::triggered, &(line->line_edit), &TextEdit::cut);
  connect(&copy_text_action, &QAction::triggered, &(line->line_edit), &TextEdit::copy);
  connect(&paste_text_action, &QAction::triggered, &(line->line_edit), &TextEdit::paste);
  connect(&select_all_text_action, &QAction::triggered, &(line->line_edit), &TextEdit::selectAll);
  addMenu(&edit_menu);
  // Extensions
  extensions_menu.setTitle(tr("Extensions"));
  extensions_viewer_action.setText(tr("Extensions viewer") + space + Basis::extensions_viewer_cmd);
  scenario_running_info.setText(tr("No scenario running"));
  scenario_running_info.setEnabled(false);
  connect(core, &Core::change_menubar_scenario_name, this, [this](QString name) {
    if (name.isEmpty()) {
      disconnect(&scenario_running_info, &QAction::triggered, nullptr, nullptr);
      scenario_running_info.setText(tr("No scenario running"));
      scenario_running_info.setEnabled(false);
    } else {
      scenario_running_info.setText(tr("Running: ") + name);
      scenario_running_info.setEnabled(true);
      connect(&scenario_running_info, &QAction::triggered, core, &Core::got_scenario_shutting);
    }
  });
  handle_scanner();
  handle_composer();
  connect(core->basis, &Basis::custom_scanner_changed, this, &MenuBar::handle_scanner);
  connect(core->basis, &Basis::custom_composer_changed, this, &MenuBar::handle_composer);
  current_scanner_info.setText(tr("No custom scanner enabled"));
  current_scanner_info.setEnabled(false);
  select_scanner_action.setText(tr("Select custom scanner..."));
  current_composer_info.setText(tr("No custom composer enabled"));
  current_composer_info.setEnabled(false);
  select_composer_action.setText(tr("Select custom composer..."));
  extensions_viewer_action.setShortcut(Qt::CTRL | static_cast<int>(Qt::Key_Slash));
  extensions_viewer_action.setIcon(
    QIcon::fromTheme("snap-extension", QIcon(":/arts/icons/16/snap-extension.svg")));
  current_scanner_info.setIcon(
    QIcon::fromTheme("edit-find", QIcon(":/arts/icons/16/edit-find.svg")));
  current_composer_info.setIcon(
    QIcon::fromTheme("curve-connector", QIcon(":/arts/icons/16/curve-connector.svg")));
  extensions_menu.addAction(&extensions_viewer_action);
  extensions_menu.addSeparator();
  extensions_menu.addAction(&scenario_running_info);
  extensions_menu.addSeparator();
  extensions_menu.addAction(&current_scanner_info);
  extensions_menu.addAction(&select_scanner_action);
  extensions_menu.addSeparator();
  extensions_menu.addAction(&current_composer_info);
  extensions_menu.addAction(&select_composer_action);
  connect(&extensions_viewer_action, &QAction::triggered, this, [this] {
    core->got_message_from_user(Basis::extensions_viewer_cmd);
  });
  connect(&scenario_running_info, &QAction::triggered, this, [this] {
    emit stop_scenario_triggered();
  });
  connect(&current_scanner_info, &QAction::triggered, this, [this] {
    core->got_message_from_user(Basis::stop_scanner_cmd);
  });
  connect(&select_scanner_action, &QAction::triggered, this, [this] {
    core->got_message_from_user(Basis::add_scanner_cmd);
  });
  connect(&current_composer_info, &QAction::triggered, this, [this] {
    core->got_message_from_user(Basis::stop_composer_cmd);
  });
  connect(&select_composer_action, &QAction::triggered, this, [this] {
    core->got_message_from_user(Basis::add_composer_cmd);
  });
  addMenu(&extensions_menu);
  // Tools
  tools_menu.setTitle(tr("Tools"));
  hide_menubar_action.setText(tr("Hide menubar"));
  full_screen_action.setText(tr("Full screen"));
  full_screen_action.setCheckable(true);
  settings_action.setText(tr("Settings...") + space + Basis::settings_cmd);
  hide_menubar_action.setShortcut(Qt::CTRL | static_cast<int>(Qt::Key_H));
  full_screen_action.setShortcut(Qt::Key_F11);
  settings_action.setShortcut(Qt::CTRL | Qt::SHIFT | Qt::Key_Comma);
  hide_menubar_action.setIcon(
    QIcon::fromTheme("show-menu", QIcon(":/arts/icons/16/show-menu.svg")));
  full_screen_action.setIcon(
    QIcon::fromTheme("view-fullscreen", QIcon(":/arts/icons/16/view-fullscreen.svg")));
  settings_action.setIcon(
    QIcon::fromTheme("configure", QIcon(":/arts/icons/16/configure.svg")));
  tools_menu.addAction(&hide_menubar_action);
  tools_menu.addAction(&full_screen_action);
  tools_menu.addSeparator();
  tools_menu.addAction(&settings_action);
  connect(&hide_menubar_action, &QAction::triggered, this, [this] { setVisible(!isVisible()); });
  connect(&settings_action, &QAction::triggered, this, [this] {
    core->got_message_from_user(Basis::settings_cmd);
  });
  addMenu(&tools_menu);
  // Help
  help_menu.setTitle(tr("Help"));
  about_jeff_action.setText(tr("About") + space + Basis::about_cmd);
  about_jeff_action.setIcon(
    QIcon::fromTheme("help-about", QIcon(":/arts/icons/16/help-about.svg")));
  help_menu.addAction(&about_jeff_action);
  help_menu.addAction(QIcon(":/arts/icons/16/qt.svg"), tr("About Qt"), &QApplication::aboutQt);
  connect(&about_jeff_action, &QAction::triggered, this, [this] {
    core->got_message_from_user(Basis::about_cmd);
  });
  addMenu(&help_menu);
}

/*! @brief TBD */
void MenuBar::handle_scanner() {
  if (not core->basis->custom_scanner) {
    disconnect(&current_scanner_info, &QAction::triggered, nullptr, nullptr);
    current_scanner_info.setText(tr("No custom scanner enabled"));
    current_scanner_info.setEnabled(false);
  } else {
    current_scanner_info.setText(tr("Running: ") + QFileInfo(core->basis->custom_scanner->origin).fileName());
    current_scanner_info.setEnabled(true);
    connect(&current_scanner_info, &QAction::triggered, core, [this] {
      core->basis->set_custom_scanner(nullptr);
    });
  }
}

/*! @brief TBD */
void MenuBar::handle_composer() {
  if (not core->basis->custom_composer) {
    disconnect(&current_composer_info, &QAction::triggered, nullptr, nullptr);
    current_composer_info.setText(tr("No custom composer enabled"));
    current_composer_info.setEnabled(false);
  } else {
    current_composer_info.setText(tr("Running: ") + QFileInfo(core->basis->custom_composer->origin).fileName());
    current_composer_info.setEnabled(true);
    connect(&current_composer_info, &QAction::triggered, core, [this] {
      core->basis->set_custom_composer(nullptr);
    });
  }
}
