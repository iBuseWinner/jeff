#ifndef A_MESSAGE_H
#define A_MESSAGE_H

#include <QDateTime>
#include <QHBoxLayout>
#include <QLabel>
#include <QPair>
#include <QSizePolicy>
#include <QSpacerItem>
#include <QTextDocument>
#include "core/message.h"
#include "widgets/a_board.h"

class AMessage : public QWidget {
  Q_OBJECT
 public:
  AMessage();
  AMessage(message _daemon);
  void setDaemon(message _daemon);
  void setWidget(QWidget *_w);
  enum A returnAuthor() { return daemon.aType; }
  enum C returnContentType() { return daemon.cType; }
  enum T returnTheme() { return daemon.tType; }
  QString returnContent() { return daemon.cn; }
  message returnDaemon() { return daemon; }

 private:
  Q_DISABLE_COPY(AMessage)
  message daemon;
  QGridLayout *el = nullptr;
  QWidget *w = nullptr;
  const int mmw = 400;
  const int bm = 6;
  void setAuthor(A _a);
  void setMessageType(C _ct);
  void setTheme(T _t);
  void setupASW();
  void setupUser();
  void setupText(const QString &_cn);
  void setupMarkdown(const QString &_cn);
  // void setupPicture(const QString &_cn);
  // void setupFile(const QString &_cn);
  void setupWarning(const QString &_cn);
  void setupError(const QString &_cn);
  void prepareSetupWidget();
  void alignTextToWindowWidth();
  QPair<QSpacerItem *, ABoard *> mk();
};

#endif  // A_MESSAGE_H
