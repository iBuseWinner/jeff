#ifndef ASW_H
#define ASW_H
#include "core/containersstruct.h"
#include "core/handlers.h"
#include "core/settingsstore.h"
#include "core/sqlite.h"
#include "core/thinking.h"
#include "dialogues/firststart.h"
#include "widgets/akiwake_board.h"
#include "widgets/akiwake_display.h"
#include "widgets/akiwake_line.h"
#include "widgets/akiwake_menubar.h"
#include "widgets/akiwake_message.h"
#include "widgets/akiwake_pushbutton.h"
#include <QApplication>
#include <QLayout>
#include <QLineEdit>
#include <QList>
#include <QMainWindow>
#include <QPair>
#include <QResizeEvent>
#include <QWidget>

class ASW : public QMainWindow {
  Q_OBJECT
public:
  ASW(QWidget *parent = nullptr);
  ~ASW();
  AkiwakeDisplay *display;
  AkiwakeLine *line;
  AkiwakeMenuBar *mBar;
  void userSendsMessage();

private:
  Q_DISABLE_COPY(ASW)
  const QString isMenubarHiddenSt = "asw/menubarishidden";
  const QString sizeSt = "asw/size";
  const QString isFullscreenSt = "asw/isfullscreen";
  const QString isNotFirstStartSt = "asw/isnotfirststart";
  void applyingSettings();
  void connector();
  void fullscreenHandler();
  void clearScreen();
  AkiwakeMessage::ThemeType themeFolder = AkiwakeMessage::Light;
  AkiwakeMessage *current = nullptr;
  thinking *TH;
  QList<AkiwakeMessage *> messages;
  void addMessage(AkiwakeMessage::AuthorType Author, QString Text);
  virtual void resizeEvent(QResizeEvent *event);
  virtual void keyPressEvent(QKeyEvent *event);
};

#endif // ASW_H
