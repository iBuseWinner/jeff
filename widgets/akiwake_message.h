#ifndef AKIWAKE_MESSAGE_H
#define AKIWAKE_MESSAGE_H

#include "widgets/akiwake_board.h"
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QResizeEvent>
#include <QSizePolicy>
#include <QSpacerItem>
#include <QWidget>

class AkiwakeMessage : public QWidget {
  Q_OBJECT
public:
  enum ThemeType { Light, Dark, Red, Green, Blue, Yellow };
  enum AuthorType { ASW, User };
  QString currentText;
  ThemeType themeFolder;
  AuthorType messageAuthor;
  bool unitedStyle = false;
  explicit AkiwakeMessage(const QString& Text, AuthorType Author, ThemeType Theme,
                          QWidget *parent = nullptr);
  QString returnCurrentText();
  void commonMaker();
  void textLayoutDesigner(int width);
  void themeUpdater();
  AkiwakeBoard *board = new AkiwakeBoard(this);
  QLabel *messageLabel;

private:
  Q_DISABLE_COPY(AkiwakeMessage)
  void authorDefine();
  void labelAppending(const QString& Text);
  void labelTextColor();
};

#endif // AKIWAKE_MESSAGE_H
