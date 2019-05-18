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

/*!
 * Class: ASW
 * Application window class.
 */
class ASW : public QMainWindow {
  Q_OBJECT
 public:
  // Functions:
  ASW();
  void greeting();

  /*! Saves window settings. */
  ~ASW() override { saveWindowSettings(); }

 signals:
  void readyState();
  QString send(QString t);

 protected:
  void keyPressEvent(QKeyEvent *event) override;

 private:
  Q_DISABLE_COPY(ASW)

  // Objects:
  ALine *ln = new ALine(this);
  ADisplay *d = new ADisplay(this);
  AMenuBar *mb = new AMenuBar(ln, this);
  core *cr = new core(this);
  const int mw = 600;
  const int mh = 370;
  const int stdw = 800;
  const int stdh = 496;
  const QString isMenuBarHiddenSt = "asw/menubarishidden";
  const QString sizeSt = "asw/size";
  const QString isFullScreenSt = "asw/isfullscreen";
  const QString isNotFirstStartSt = "asw/isnotfirststart";

  // Functions:
  void applyingSettings();
  void saveWindowSettings();
  void connector();
  void fullScreenHandler();
  void userInputHandler();
  void exportMessageHistory();
  void clear();
  void addMessage(AMessage *msg);

  void about() { emit send("/about"); }
  void containerManager() { emit send("/cm"); }
  void firstStart() { emit send("/first"); }
  // void help() { emit send("/help"); }

};

#endif  // ASW_H
