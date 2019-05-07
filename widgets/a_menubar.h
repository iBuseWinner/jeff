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
  QAction *fsa = nullptr;
  AMenuBar(ALine *l, QWidget *p = nullptr);
  ~AMenuBar() override;

 signals:
  void fullscreenModeChanged();
  void clearHistoryTriggered();
  void aboutTriggered();
  void containersTriggered();
  void helpTriggered();
  void exportTriggered();

 private:
  Q_DISABLE_COPY(AMenuBar)
  bool fs = 0;
  QMenu *mf = nullptr;
  QMenu *me = nullptr;
  QMenu *mt = nullptr;
  QMenu *mh = nullptr;
  void openContainerManager();
  void openAbout();
  void exportMessageHistory();
  void clearMessageHistory();
  void fScreen();
  void hideThis();
};

#endif  // AKIWAKE_MENUBAR_H
