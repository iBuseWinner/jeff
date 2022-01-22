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
public:
  // Objects:
  QAction *fullScreenAction = nullptr;
  QAction *emm = nullptr;

  // Functions described in 'menubar.cpp':
  explicit MenuBar(Line *line, QWidget *parent = nullptr);

signals:
  /*!
   * @brief Informs about clicking on the "Clear history" action.
   */
  void clear_history_triggered();

  /*!
   * @brief Informs about clicking on the "About" action.
   */
  void about_triggered();

  /*!
   * @brief Informs about clicking on the "Settings" action.
   */
  void settings_triggered();

  /*!
   * @brief Informs about clicking on the "Source manager" action.
   */
  void sources_triggered();

  /*!
   * @brief Informs about clicking on the "Help" action.
   * @details [constructing]
   * TODO
   */
  // void help_triggered();

  /*!
   * @brief Informs about clicking on the "Export message history..." action.
   */
  void export_triggered();

  /*!
   * @brief Informs about clicking on the "Import message history..." action.
   */
  void import_triggered();

private:
  Q_DISABLE_COPY(MenuBar)
};

#endif
