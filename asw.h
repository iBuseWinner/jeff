#ifndef ASW_H
#define ASW_H
#include "core/containersstruct.h"
#include "core/handlers.h"
#include "core/settingsstore.h"
#include "core/sqlite.h"
#include "core/thinking.h"
#include "dialogues/firststart.h"
#include "widgets/a_board.h"
#include "widgets/a_display.h"
#include "widgets/a_line.h"
#include "widgets/a_menubar.h"
#include "widgets/a_message.h"
#include "widgets/a_pushbutton.h"
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
  ADisplay *display = nullptr;
  ALine *line = nullptr;
  AMenuBar *mBar = nullptr;
  void greeting();

signals:
  void readyState();

protected:
  void resizeEvent(QResizeEvent *event) override;
  void keyPressEvent(QKeyEvent *event) override;

private:
  Q_DISABLE_COPY(ASW)
  SettingsStore *ST = new SettingsStore();
  const QString isMenubarHiddenSt = "asw/menubarishidden";
  const QString sizeSt = "asw/size";
  const QString isFullscreenSt = "asw/isfullscreen";
  const QString isNotFirstStartSt = "asw/isnotfirststart";
  void applyingSettings();
  void saveSettings();
  void connector();
  void fullscreenHandler();
  void userSendsMessage();
  void clearScreen();
  void fsStarter();
  void aboutStarter();
  void cmStarter();
  void helpStarter();
  AMessage::T m_theme = AMessage::Light;
  thinking *TH{};
  QList<AMessage *> messages;
  void addMessage(AMessage::A Author, const QString &Text);
};

#endif // ASW_H
