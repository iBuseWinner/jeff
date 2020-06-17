#ifndef A_MESSAGE_H
#define A_MESSAGE_H

#include "core/message.h"
#include "widgets/a_board.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QPair>
#include <QSizePolicy>
#include <QSpacerItem>
#include <QTextDocument>

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
  void setShadow(Message _message);
  void setWidget(QWidget *_widget);

  Author returnAuthor() { return message.aType; }
  ContentType returnContentType() { return message.cType; }
  Theme returnTheme() { return message.tType; }
  QString returnContent() { return message.content; }
  QDateTime returnDateTime() { return message.datetime; }
  Message returnShadow() { return message; }

signals:
  void closed();

private:
  Q_DISABLE_COPY(AMessage)

  // Objects:
  Message message;
  QGridLayout *gridLayout = nullptr;
  QWidget *widget = nullptr;
  static const ushort maximalWidgetWidth = 800;
  static const ushort standardMargin = 6;

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
  QPair<QSpacerItem *, ABoard *> makeLayout();
};

#endif // A_MESSAGE_H
