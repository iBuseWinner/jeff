#include "menubar.h"

/*!
 * @fn MenuBar::MenuBar
 * @brief The constructor.
 * @param[in,out] line reference to the Line instance
 * @param[in,out] parent QObject parent
 */
MenuBar::MenuBar(Line *line, QWidget *parent) : QMenuBar(parent) {
  // File
  file_menu.setTitle(tr("File"));
  source_manager_action.setText(tr("Source manager") + " (/sourcemanager)");
  export_history_action.setText(tr("Export message history"));
  import_history_action.setText(tr("Import message history"));
  enable_monologue_mode.setText(tr("Enable monologue mode") + " (/mm)");
  enable_monologue_mode.setCheckable(true);
  source_manager_action.setShortcut(Qt::CTRL | static_cast<int>(Qt::Key_M));
  export_history_action.setShortcut(Qt::CTRL | static_cast<int>(Qt::Key_E));
  import_history_action.setShortcut(Qt::CTRL | static_cast<int>(Qt::Key_I));
  enable_monologue_mode.setShortcut(Qt::CTRL | Qt::SHIFT | Qt::Key_M);
  source_manager_action.setIcon(
    QIcon::fromTheme("network-server-database", QIcon(":/arts/icons/16/database-manager.svg")));
  export_history_action.setIcon(
    QIcon::fromTheme("document-export.svg", QIcon(":/arts/icons/16/document-export.svg")));
  import_history_action.setIcon(
    QIcon::fromTheme("document-import.svg", QIcon(":/arts/icons/16/document-import.svg")));
  enable_monologue_mode.setIcon(
    QIcon::fromTheme("user-group-properties", QIcon(":/arts/icons/16/monologue.svg")));
  file_menu.addAction(&source_manager_action);
  file_menu.addAction(&enable_monologue_mode);
  file_menu.addSeparator();
  file_menu.addAction(&export_history_action);
  file_menu.addAction(&import_history_action);
  file_menu.addSeparator();
  file_menu.addAction(
    QIcon::fromTheme("application-exit", QIcon(":/arts/icons/16/application-exit.svg")),
    tr("&Exit"), &QApplication::quit, Qt::ALT | static_cast<int>(Qt::Key_F4));
  connect(&source_manager_action, &QAction::triggered, this, [this] { emit sources_triggered(); });
  connect(&export_history_action, &QAction::triggered, this, [this] { emit export_triggered(); });
  connect(&import_history_action, &QAction::triggered, this, [this] { emit import_triggered(); });
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
  connect(&delete_text_action, &QAction::triggered, &(line->line_edit), &LineEdit::backspace);
  connect(&cut_text_action, &QAction::triggered, &(line->line_edit), &LineEdit::cut);
  connect(&copy_text_action, &QAction::triggered, &(line->line_edit), &LineEdit::copy);
  connect(&paste_text_action, &QAction::triggered, &(line->line_edit), &LineEdit::paste);
  connect(&select_all_text_action, &QAction::triggered, &(line->line_edit), &LineEdit::selectAll);
  addMenu(&edit_menu);
  // Tools
  tools_menu.setTitle(tr("Tools"));
  hide_menubar_action.setText(tr("Hide menubar"));
  full_screen_action.setText(tr("Full screen"));
  full_screen_action.setCheckable(true);
  settings_action.setText(tr("Settings...") + " (/settings)");
  hide_menubar_action.setShortcut(Qt::CTRL | static_cast<int>(Qt::Key_H));
  full_screen_action.setShortcut(Qt::Key_F11);
  settings_action.setShortcut(Qt::CTRL | Qt::SHIFT | Qt::Key_Comma);
  hide_menubar_action.setIcon(
    QIcon::fromTheme("show-menu.svg", QIcon(":/arts/icons/16/show-menu.svg")));
  full_screen_action.setIcon(
    QIcon::fromTheme("view-fullscreen.svg", QIcon(":/arts/icons/16/view-fullscreen.svg")));
  settings_action.setIcon(
    QIcon::fromTheme("configure.svg", QIcon(":/arts/icons/16/configure.svg")));
  tools_menu.addAction(&hide_menubar_action);
  tools_menu.addAction(&full_screen_action);
  tools_menu.addSeparator();
  tools_menu.addAction(&settings_action);
  connect(&hide_menubar_action, &QAction::triggered, this, [this] { setVisible(!isVisible()); });
  connect(&settings_action, &QAction::triggered, this, [this] { emit settings_triggered(); });
  addMenu(&tools_menu);
  // Help
  help_menu.setTitle(tr("Help"));
  about_action.setText(tr("About") + " (/about)");
  about_action.setIcon(
    QIcon::fromTheme("help-about", QIcon(":/arts/icons/16/help-about.svg")));
  help_menu.addAction(&about_action);
  help_menu.addAction(QIcon(":/arts/icons/16/qt.svg"), tr("About Qt"), &QApplication::aboutQt);
  connect(&about_action, &QAction::triggered, this, [this] { emit about_triggered(); });
  addMenu(&help_menu);
}
