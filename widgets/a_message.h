#ifndef A_MESSAGE_H
#define A_MESSAGE_H

#include "core/message.h"
#include "dialogues/modal-handler.h"
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
  // Objects:
  static const ushort maximalMessageWidth = 400;

  // Functions:
  AMessage();
  AMessage(Message message);
  void setMessage(Message _message);
  void setWidget(ModalHandler *m_handler);
  Author returnAuthor() { return message.author; }
  ContentType returnContentType() { return message.content_type; }
  Theme returnTheme() { return message.theme; }
  QString returnContent() { return message.content; }
  QDateTime returnDateTime() { return message.datetime; }
  Message returnMessage() { return message; }

signals:
  void closed();

private:
  Q_DISABLE_COPY(AMessage)

  // Objects:
  Message message;
  QGridLayout *gridLayout = nullptr;

  QWidget *widget = nullptr;
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
