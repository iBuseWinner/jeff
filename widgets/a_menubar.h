#ifndef A_MENUBAR_H
#define A_MENUBAR_H

#include "widgets/a_line.h"
#include <QAction>
#include <QApplication>
#include <QJsonArray>
#include <QMenu>
#include <QMenuBar>
#include <QWidget>

/*!
 * @class AMenuBar
 * @brief Provides the ability to interact with ASW using the menubar.
 */
class AMenuBar : public QMenuBar {
  Q_OBJECT
public:
  // Objects:
  QAction *fullScreenAction = nullptr;
  QAction *emm = nullptr;

  // Functions described in 'a_menubar.cpp':
  explicit AMenuBar(ALine *line, QWidget *parent = nullptr);

signals:
  /*!
   * @brief Informs about clicking on the "Clear history" action.
   */
  void clearHistoryTriggered();

  /*!
   * @brief Informs about clicking on the "About" action.
   */
  void aboutTriggered();

  /*!
   * @brief Informs about clicking on the "Settings" action.
   */
  void settingsTriggered();

  /*!
   * @brief Informs about clicking on the "Source manager" action.
   */
  void sourcesTriggered();

  /*!
   * @brief Informs about clicking on the "Help" action.
   * @details [constructing]
   */
  // void helpTriggered();

  /*!
   * @brief Informs about clicking on the "Export message history..." action.
   */
  void exportTriggered();

  /*!
   * @brief Informs about clicking on the "Import message history..." action.
   */
  void importTriggered();

private:
  Q_DISABLE_COPY(AMenuBar)
};

#endif // A_MENUBAR_H
