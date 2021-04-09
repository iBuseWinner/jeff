#ifndef A_MESSAGE_H
#define A_MESSAGE_H

#include "core/model/message.h"
#include "dialogues/modal-handler.h"
#include "widgets/a_board.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QPair>
#include <QSizePolicy>
#include <QSpacerItem>
#include <QTextDocument>

/*!
 * @class AMessage
 * @brief Widget that displays single piece of data.
 * @details Any text entered from the keyboard or NLPmodule/PythonHandler,
 * widget, picture, audio, file, etc. can be used as a message.
 */
class AMessage : public QWidget {
  Q_OBJECT
public:
  // Constants:
  static const ushort maximalMessageWidth = 400;

  // Functions:
  Author returnAuthor() { return message.author; }
  ContentType returnContentType() { return message.content_type; }
  Theme returnTheme() { return message.theme; }
  QString returnContent() { return message.content; }
  QDateTime returnDateTime() { return message.datetime; }
  Message returnMessage() { return message; }

  // Functions described in 'a_message':
  explicit AMessage();
  explicit AMessage(Message message);
  void setMessage(Message _message);
  void setWidget(ModalHandler *m_handler);

signals:
  /*!
   * @brief When the message is closed, it can inform ModalHandler, and then it
   * will delete the message from ADisplay.
   */
  void closed();

private:
  Q_DISABLE_COPY(AMessage)

  // Objects:
  Message message;
  QGridLayout *gridLayout = nullptr;
  QWidget *widget = nullptr;

  // Constants:
  static const ushort standardMargin = 6;

  // Functions described in 'a_message.cpp':
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
