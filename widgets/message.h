#ifndef MESSAGE_H
#define MESSAGE_H

#include "core/model/message.h"
#include "dialogues/modal-handler.h"
#include "widgets/board.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QPair>
#include <QResizeEvent>
#include <QSizePolicy>
#include <QSpacerItem>
#include <QTextDocument>

/*!
 * @class Message
 * @brief Widget that displays single piece of data.
 * @details Any text entered from the keyboard or NLPmodule/PythonHandler,
 * widget, picture, audio, file, etc. can be used as a message.
 */
class Message : public QWidget {
  Q_OBJECT
  Q_DISABLE_COPY(Message)
public:
  // Functions:
  Author returnAuthor() { return message.author; }
  ContentType returnContentType() { return message.content_type; }
  Theme returnTheme() { return message.theme; }
  QString returnContent() { return message.content; }
  QDateTime returnDateTime() { return message.datetime; }
  MessageData returnMessage() { return message; }

  // Functions described in 'message.cpp':
  Message();
  Message(MessageData message);
  void setMessage(MessageData _message);
  void setWidget(ModalHandler *m_handler);

signals:
  /*!
   * @brief When the message is closed, it can inform ModalHandler, and then it
   * will delete the message from Display.
   */
  void closed();

private:
  // Objects:
  MessageData message;
  QGridLayout *gridLayout = nullptr;
  QWidget *widget = nullptr;

  // Constants:
  static const ushort standardMargin = 6;

  // Functions described in 'message.cpp':
  void setAuthor(Author aType);
  void setMessageType(ContentType cType);
  // void setTheme(eT tType);
  void setupJeff();
  void setupUser();
  void setupText(const QString &content);
  void setupMarkdown(const QString &content);
  // void setupPicture(const QString &content);
  // void setupFile(const QString &content);
  void setupWarning(const QString &content);
  void setupError(const QString &content);
  void prepareSetupWidget();
  void resizeEvent(QResizeEvent *event) override;
  QPair<QSpacerItem *, Board *> makeLayout();
};

#endif
