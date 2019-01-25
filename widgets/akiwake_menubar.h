#ifndef AKIWAKE_MENUBAR_H
#define AKIWAKE_MENUBAR_H

#include "dialogues/containers.h"
#include "widgets/akiwake_line.h"
#include <QAction>
#include <QApplication>
#include <QMenu>
#include <QMenuBar>
#include <QWidget>

class AkiwakeMenuBar : public QMenuBar {
  Q_OBJECT
public:
  explicit AkiwakeMenuBar(AkiwakeLine *line, QWidget *parent = nullptr);
  ~AkiwakeMenuBar() override;

signals:
  void fullscreenModeChanged(bool mode);
  void clearScreenPressed();

private:
  Q_DISABLE_COPY(AkiwakeMenuBar)
  bool m_isFullscreen = 0;
  void openContainerManager();
  void saveToFile();
  void clearScreen();
  void fScreen();
  void hideThis();
  QMenu *menuFile;
  QMenu *menuEdit;
  QMenu *menuTools;
  QAction *fullScreen;
};

#endif // AKIWAKE_MENUBAR_H
