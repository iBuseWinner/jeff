#ifndef A_MENUBAR_H
#define A_MENUBAR_H

#include "dialogues/about.h"
#include "dialogues/containers.h"
#include "widgets/a_line.h"
#include <QAction>
#include <QApplication>
#include <QJsonArray>
#include <QMenu>
#include <QMenuBar>
#include <QWidget>

class AMenuBar : public QMenuBar {
  Q_OBJECT
public:
  AMenuBar(ALine *line, QWidget *parent = nullptr);
  ~AMenuBar() override;
  QAction *fullScreen;

signals:
  void fullscreenModeChanged();
  void clearScreenTriggered();
  void aboutTriggered();
  void contManTriggered();
  void helpTriggered();
  void saveToFileTriggered();

private:
  Q_DISABLE_COPY(AMenuBar)
  bool m_isFullscreen = 0;
  void openContainerManager();
  void openAbout();
  void saveToFile();
  void clearScreen();
  void fScreen();
  void hideThis();
  QMenu *menuFile;
  QMenu *menuEdit;
  QMenu *menuTools;
  QMenu *menuHelp;
};

#endif // AKIWAKE_MENUBAR_H
