#ifndef MESSAGE_H
#define MESSAGE_H

#include <QAction>
#include <QClipboard>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QLabel>
#include <QPair>
#include <QPoint>
#include <QResizeEvent>
#include <QSizePolicy>
#include <QSpacerItem>
#include <QTextDocument>
#include "core/model/message.h"
#include "dialogues/modal-handler.h"
#include "widgets/board.h"
#include "widgets/menu.h"

/*! @class Message
 *  @brief Widget that displays single piece of data.
 *  @details Any text entered from the keyboard or NLPmodule/PythonHandler,
 *  widget, picture, audio, file, etc. can be used as a message.  */
class Message : public QWidget {
  Q_OBJECT
  Q_DISABLE_COPY(Message)
public:
  // Functions:
  Author author() { return md.author; }
  ContentType content_type() { return md.content_type; }
  Theme theme() { return md.theme; }
  QString content() { return md.content; }
  QDateTime date_time() { return md.datetime; }
  MessageData message_data() { return md; }

  // Functions described in `message.cpp`:
  Message();
  Message(MessageData _md);
  void message_data(MessageData _md);
  void widget(ModalHandler *modal_handler);
  void update_text(const QString &text);
  void setWidth(int width);

signals:
  /*! @brief When the message is closed, it can inform ModalHandler, and then it
   *  will delete the message from Display.  */
  void closed();

private:
  // Objects:
  MessageData md;
  QGridLayout *grid_layout = nullptr;
  QWidget *w = nullptr;
  int _width;

  // Constants:
  static const ushort standardMargin = 6;

  // Functions described in `message.cpp`:
  void author(Author _a);
  void content_type(ContentType _ct);
  void theme(Theme _t);
  void setup_jeff();
  void setup_user();
  void setup_text(const QString &content);
  void setup_markdown(const QString &content);
  void setup_picture(const QString &content);
  // void setup_file(const QString &content);
  void setup_warning(const QString &content);
  void setup_error(const QString &content);
  void prepare_to_widget();
  QPair<QSpacerItem *, Board *> make_layout();
};

#endif
