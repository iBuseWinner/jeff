#ifndef A_MESSAGE_H
#define A_MESSAGE_H

#include <QHBoxLayout>
#include <QLabel>
#include <QPair>
#include <QSizePolicy>
#include <QSpacerItem>
#include <QTextDocument>
#include "core/message.h"
#include "widgets/a_board.h"

/*!
 * Class: AMessage.
 * Widget that displays data.
 */
class AMessage : public QWidget {
  Q_OBJECT
 public:
  // Functions:
  AMessage();
  AMessage(Message shadow);
  void setShadow(Message shadow);
  void setWidget(QWidget *widget);

  Author returnAuthor() { return sh.aType; }
  ContentType returnContentType() { return sh.cType; }
  Theme returnTheme() { return sh.tType; }
  QString returnContent() { return sh.content; }
  QDateTime returnDateTime() { return sh.datetime; }
  Message returnShadow() { return sh; }

 private:
  Q_DISABLE_COPY(AMessage)

  // Objects:
  Message sh;
  QGridLayout *lt = nullptr;
  QWidget *w = nullptr;
  const int mmw = 400;
  const int stdm = 6;

  // Functions:
  void setAuthor(Author aType);
  void setMessageType(ContentType cType);
  // void setTheme(eT tType);
  void setupASW();
  void setupUser();
  void setupText(const QString &content);
  void setupMarkdown(const QString &content);
  // void setupPicture(const QString &content);
  // void setupFile(const QString &content);
  void setupWarning(const QString &content);
  void setupError(const QString &content);
  void prepareSetupWidget();
  void alignTextToWindowWidth();
  QPair<QSpacerItem *, ABoard *> mkLt();
};

#endif  // A_MESSAGE_H
