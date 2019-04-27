#ifndef A_MESSAGE_H
#define A_MESSAGE_H

#include <QDateTime>
#include <QFont>
#include <QFontMetrics>
#include <QHBoxLayout>
#include <QLabel>
#include <QResizeEvent>
#include <QSizePolicy>
#include <QSpacerItem>
#include <QWidget>
#include "widgets/a_board.h"

class AMessage : public QWidget {
  Q_OBJECT
 public:
  enum A { undefA, ASW, User };
  enum CT { undefCT, Text, HTML, Widget, Picture, File };
  enum T { undefT, Light, Dark, Red, Green, Blue, Yellow };
  AMessage(QWidget *parent = nullptr);
  void setAuthor(A _a);
  void setMessageType(CT _ct, const QString &_cn);
  void setMessageType(CT _ct, QWidget *_cn);
  void setTheme(T _t);
  enum A returnAuthor() { return a; }
  enum CT returnMessageType() { return ct; }
  enum T returnTheme() { return t; }
  QString returnText();

 private:
  Q_DISABLE_COPY(AMessage)
  const int bm = 6;
  const int mmw = 400;
  A a = A::undefA;
  CT ct = CT::undefCT;
  T t = T::undefT;
  QHBoxLayout *el = new QHBoxLayout();
  ABoard *b = new ABoard(this);
  QString cn = "";
  QLabel *tl = nullptr;
  void createASWLayout();
  void createUserLayout();
  void setupPlainMessage(const QString &_cn);
  void setupHTMLMessage(const QString &_cn);
  void setupWidgetMessage(QWidget *_cn);
  // void setupMessage_pict(QString path);
  // void setupMessage_file(QString path);
  void alignTextToWindowWidth();
};

#endif  // A_MESSAGE_H
