#ifndef ASW_H
#define ASW_H

#include <QApplication>
#include <QLayout>
#include <QLineEdit>
#include <QList>
#include <QMainWindow>
#include <QPair>
#include <QResizeEvent>
#include <QWidget>
#include "core/core.h"
#include "widgets/a_board.h"
#include "widgets/a_button.h"
#include "widgets/a_display.h"
#include "widgets/a_line.h"
#include "widgets/a_menubar.h"

class ASW : public QMainWindow {
  Q_OBJECT
 public:
  ADisplay *d = new ADisplay(this);
  ALine *l = new ALine(this);
  AMenuBar *mb = new AMenuBar(l, this);
  ASW();
  void greeting();

 signals:
  void readyState();
  QString send(QString t);

 protected:
  void keyPressEvent(QKeyEvent *event) override;

 private:
  Q_DISABLE_COPY(ASW)
  core *cr = new core(this);
  const int mw = 600;
  const int mh = 370;
  const int stdw = 800;
  const int stdh = 496;
  const QString isMenubarHiddenSt = "asw/menubarishidden";
  const QString sizeSt = "asw/size";
  const QString isFullscreenSt = "asw/isfullscreen";
  const QString isNotFirstStartSt = "asw/isnotfirststart";
  void applyingSettings();
  void saveSettings();
  void connector();
  void fsh();
  void userInput();
  void clearScreen();
  void fsStarter();
  void aboutStarter();
  void cmStarter();
  // void helpStarter();
  void addMessage(AMessage *msg);
};

#endif  // ASW_H
