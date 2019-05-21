#ifndef A_MENUBAR_H
#define A_MENUBAR_H

#include <QAction>
#include <QApplication>
#include <QJsonArray>
#include <QMenu>
#include <QMenuBar>
#include <QWidget>
#include "dialogues/about.h"
#include "dialogues/containers.h"
#include "widgets/a_line.h"

/*!
 * Class: AMenuBar.
 * Provides the ability to interact with ASW using the menubar.
 */
class AMenuBar : public QMenuBar {
  Q_OBJECT
 public:
  // Objects:
  QAction *fullScreenAction = nullptr;
  QAction *emm = nullptr;

  // Functions:
  AMenuBar(ALine *line, bool isMonologueModeEnabled = false,
           QWidget *parent = nullptr);

 signals:
  void clearHistoryTriggered();
  void aboutTriggered();
  void settingsTriggered();
  void containersTriggered();
  // void helpTriggered();
  void exportTriggered();
  void importTriggered();

 private:
  Q_DISABLE_COPY(AMenuBar)

  // Functions:
  void openContainerManager() { emit containersTriggered(); }
  void openAbout() { emit aboutTriggered(); }
  void openSettings() { emit settingsTriggered(); }
  void exportMessageHistory() { emit exportTriggered(); }
  void importMessageHistory() { emit importTriggered(); }
  void clearMessageHistory() { emit clearHistoryTriggered(); }
  void hideThis() { setVisible(!isVisible()); }
};

#endif  // AKIWAKE_MENUBAR_H
