#ifndef AKIWAKE_MESSAGE_H
#define AKIWAKE_MESSAGE_H

#include "widgets/akiwake_board.h"
#include <QFontDatabase>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPropertyAnimation>
#include <QResizeEvent>
#include <QSizePolicy>
#include <QSpacerItem>
#include <QWidget>

class AkiwakeMessage : public QWidget {
  Q_OBJECT
public:
  enum ThemeType { Light, Dark, Red, Green, Blue, Yellow };
  enum AuthorType { ASW, User };
  ~AkiwakeMessage();
  QString currentText;
  ThemeType themeFolder;
  AuthorType messageAuthor;
  bool unitedStyle = false;
  explicit AkiwakeMessage(QString Text, AuthorType Author, ThemeType Theme,
                          QWidget *parent = nullptr);
  QString returnCurrentText();
  void commonMaker();
  void textLayoutDesigner(int width);
  void themeUpdater();
  AkiwakeBoard *board = new AkiwakeBoard();
  QLabel *messageLabel;

private:
  Q_DISABLE_COPY(AkiwakeMessage)
  QPropertyAnimation *Animation;
  QLabel *getMessageLabel(QString Text);
  void authorDefine();
  void labelAppending(QString Text);
  void labelTextColor();
};

#endif // AKIWAKE_MESSAGE_H
