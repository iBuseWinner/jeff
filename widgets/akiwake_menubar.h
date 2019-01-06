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
public:
  explicit AkiwakeMenuBar(AkiwakeLine *line,QWidget *parent = nullptr);

private:
  Q_DISABLE_COPY(AkiwakeMenuBar)
  void openContainerManager();
  void saveToFile();
  void clearScreen();
  void fScreen();

};

#endif // AKIWAKE_MENUBAR_H
