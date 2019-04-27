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
#include "core/containersstruct.h"
#include "core/handlers.h"
#include "core/nlp-module.h"
#include "core/settingsstore.h"
#include "core/sqlite.h"
#include "dialogues/firststart.h"
#include "widgets/a_board.h"
#include "widgets/a_display.h"
#include "widgets/a_line.h"
#include "widgets/a_menubar.h"
#include "widgets/a_message.h"
#include "widgets/a_pushbutton.h"

class ASW : public QMainWindow {
  Q_OBJECT
 public:
  ASW(QWidget *p = nullptr);
  ADisplay *d = nullptr;
  ALine *l = nullptr;
  AMenuBar *mb = nullptr;
  void greeting();

 signals:
  void readyState();

 protected:
  void keyPressEvent(QKeyEvent *event) override;

 private:
  Q_DISABLE_COPY(ASW)
  SettingsStore *st = new SettingsStore();
  const int mw = 600;
  const int mh = 370;
  const int currw = 800;
  const int currh = 496;
  const QString isMenubarHiddenSt = "asw/menubarishidden";
  const QString sizeSt = "asw/size";
  const QString isFullscreenSt = "asw/isfullscreen";
  const QString isNotFirstStartSt = "asw/isnotfirststart";
  void applyingSettings();
  void saveSettings();
  void connector();
  void fullScreenHandler();
  void userSendsMessage();
  void clearScreen();
  void fsStarter();
  void aboutStarter();
  void cmStarter();
  void helpStarter();
  AMessage::T t = AMessage::Light;
  NLPmodule *nm{};
  QList<AMessage *> ms;
  void addMessage(AMessage::A a, const QString &_t);
};

#endif  // ASW_H
