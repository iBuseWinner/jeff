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
 * Class: ASW.
 * Application window class.
 */
class ASW : public QMainWindow {
  Q_OBJECT
 public:
  // Functions:
  explicit ASW();
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
  core *cr = new core(this);
  ALine *ln = new ALine(this);
  ADisplay *d = new ADisplay(this);
  AMenuBar *mb = new AMenuBar(
      ln, cr->Meths->read(cr->Meths->isMonologueModeEnabledSt).toBool(), this);
  const int mw = 600;
  const int mh = 370;
  const int stdw = 800;
  const int stdh = 496;

  // Functions:
  void applyingSettings();
  void saveWindowSettings();
  void connector();
  void fullScreenHandler();
  void userInputHandler();
  void exportMessageHistory();
  void importMessageHistory();
  void clear();
  bool findASWCommand(const QString &text);

  /*! Adds {msg} to the screen. */
  void addMessage(AMessage *msg) { d->addMessage(msg); }

  void about() { emit send("/about"); }
  void containerManager() { emit send("/cm"); }
  void firstStart() { emit send("/first"); }
  // void help() { emit send("/help"); }
  void settings() { emit send("/settings"); }
};

#endif  // ASW_H
