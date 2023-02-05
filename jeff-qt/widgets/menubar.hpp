#ifndef MENUBAR_H
#define MENUBAR_H

#include "core-kit/basis.hpp"
#include "core-kit/core.hpp"
#include "widgets/line.hpp"
#include "widgets/menu.hpp"
#include <QAction>
#include <QApplication>
#include <QJsonArray>
#include <QMenu>
#include <QMenuBar>
#include <QWidget>

/*! @class MenuBar
 *  @brief Provides the ability to interact with Jeff using the menubar.  */
class MenuBar : public QMenuBar {
  Q_OBJECT
  Q_DISABLE_COPY(MenuBar)
public:
  // Objects:
  QAction
    full_screen_action, enable_monologue_mode, scenario_running_info,
    current_scanner_info, current_composer_info;

  // Functions described in `menubar.cpp`:
  MenuBar(Core *_core, Line *line, QWidget *parent = nullptr);

signals:
  void clear_history_triggered();
  void export_triggered();
  void import_triggered();
  void stop_scenario_triggered();
  void exit_triggered();
  // void help_triggered();

private:
  // Objects:
  Core *core = nullptr;
  Menu file_menu, edit_menu, extensions_menu, tools_menu, help_menu;
  QAction 
    /*! `File`  menu. */
    source_manager_action, script_manager_action, phrase_editor_action, export_history_action,
    import_history_action, exit_action,
    /*! `Edit`  menu. */
    clear_history_action, delete_text_action, cut_text_action, copy_text_action, paste_text_action,
    select_all_text_action,
    /*! `Extensions` menu. */
    extensions_viewer_action, select_scanner_action, select_composer_action,
    /*! `Tools` menu. */
    hide_menubar_action, settings_action,
    /*! `Help`  menu. */
    novice_guide, database_guide, scripts_guide, extensions_guide, about_jeff_action;
};

#endif
