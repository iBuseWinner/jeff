#ifndef ASW_H
#define ASW_H
#include "core/containersstruct.h"
#include "core/handlers.h"
#include "core/settingsstore.h"
#include "core/sqlite.h"
#include "core/thinking.h"
#include "dialogues/containers.h"
#include "widgets/akiwake_board.h"
#include "widgets/akiwake_display.h"
#include "widgets/akiwake_line.h"
#include "widgets/akiwake_message.h"
#include "widgets/akiwake_pushbutton.h"
#include "widgets/akiwake_menubar.h"
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
  void themeUpdater();

private:
  Q_DISABLE_COPY(ASW)
  void connector();
  AkiwakeMessage::ThemeType themeFolder = AkiwakeMessage::Light;
  AkiwakeMessage *current = nullptr;
  QList<AkiwakeMessage *> messages;
  void addMessage(AkiwakeMessage::AuthorType Author, QString Text);
  virtual void resizeEvent(QResizeEvent *event);
  virtual void closeEvent(QCloseEvent *event);
};

#endif // ASW_H
