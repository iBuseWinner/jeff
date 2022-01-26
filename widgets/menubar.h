#ifndef MENUBAR_H
#define MENUBAR_H

#include "widgets/line.h"
#include "widgets/menu.h"
#include <QAction>
#include <QApplication>
#include <QJsonArray>
#include <QMenu>
#include <QMenuBar>
#include <QWidget>

/*!
 * @class MenuBar
 * @brief Provides the ability to interact with Jeff using the menubar.
 */
class MenuBar : public QMenuBar {
  Q_OBJECT
  Q_DISABLE_COPY(MenuBar)
public:
  // Objects:
  QAction full_screen_action;
  QAction enable_monologue_mode;

  // Functions described in `menubar.cpp`:
  MenuBar(Line *line, QWidget *parent = nullptr);

signals:
  /*! @brief Informs about clicking on the "Clear history" action. */
  void clear_history_triggered();
  /*! @brief Informs about clicking on the "About" action. */
  void about_triggered();
  /*! @brief Informs about clicking on the "Settings" action. */
  void settings_triggered();
  /*! @brief Informs about clicking on the "Source manager" action. */
  void sources_triggered();
  /*! @brief Informs about clicking on the "Export message history..." action. */
  void export_triggered();
  /*! @brief Informs about clicking on the "Import message history..." action. */
  void import_triggered();

  /*!
   * @brief Informs about clicking on the "Help" action.
   * @details [constructing]
   * TODO
   */
  // void help_triggered();

private:
  // Objects:
  Menu file_menu, edit_menu, tools_menu, help_menu;
  QAction source_manager_action, export_history_action, import_history_action, /*!< `File`  menu. */
    clear_history_action, delete_text_action, cut_text_action,                 /*!< `Edit`  menu. */
    copy_text_action, paste_text_action, select_all_text_action,
    hide_menubar_action, settings_action,                                      /*!< `Tools` menu. */
    about_action;                                                              /*!< `Help`  menu. */
};

#endif
