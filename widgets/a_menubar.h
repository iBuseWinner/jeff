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

class AMenuBar : public QMenuBar {
  Q_OBJECT
 public:
  AMenuBar(ALine *l, QWidget *p = nullptr);
  ~AMenuBar() override;
  QAction *fsa;

 signals:
  void fullscreenModeChanged();
  void clearScreenTriggered();
  void aboutTriggered();
  void contManTriggered();
  void helpTriggered();
  void saveToFileTriggered();

 private:
  Q_DISABLE_COPY(AMenuBar)
  bool fs = 0;
  void openContainerManager();
  void openAbout();
  void saveToFile();
  void clearMsgHist();
  void fScreen();
  void hideThis();
  QMenu *mf;
  QMenu *me;
  QMenu *mt;
  QMenu *mh;
};

#endif  // AKIWAKE_MENUBAR_H
