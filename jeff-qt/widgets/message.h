#ifndef MESSAGE_W_H
#define MESSAGE_W_H

#include "core-kit/model/message.h"
#include "dialogues/modal-handler.h"
#include "maddy/parser.h"
#include "widgets/board.h"
#include "widgets/menu.h"
#include "widgets/layouts/grid.h"
#include "widgets/layouts/linears.h"
#include <string>
#include <QAction>
#include <QClipboard>
#include <QDesktopServices>
#include <QFileInfo>
#include <QLabel>
#include <QPair>
#include <QPoint>
#include <QResizeEvent>
#include <QSizePolicy>
#include <QSpacerItem>
#include <QTextDocument>
#include <QUrl>

/*! @class Message
 *  @brief Widget that displays single piece of data.
 *  @details Any text entered from the keyboard or NLPmodule/PythonHandler,
 *  widget, picture, audio, file, etc. can be used as a message.  */
class Message : public QWidget {
  Q_OBJECT
  Q_DISABLE_COPY(Message)
public:
  // Functions:
  MessageMeta message_data() { return md; }

  // Functions described in `message.cpp`:
  Message(maddy::Parser *_markdown_parser);
  Message(maddy::Parser *_markdown_parser, MessageMeta _md);
  void message_data(MessageMeta _md);
  void widget(ModalHandler *modal_handler);
  void update_text(const QString &text);
  void setWidth(int width);

signals:
  /*! @brief When the message is closed, it can inform ModalHandler, and then it
   *  will delete the message from Display.  */
  void closed();

private:
  // Objects:
  maddy::Parser *markdown_parser = nullptr;
  MessageMeta md;
  GridLt *grid_layout = nullptr;
  QWidget *w = nullptr;
  int _width;
  bool non_ideal_width_completed = false;

  // Constants:
  static const ushort standardMargin = 6;
  static const bool text_ideal_width = false;

  // Functions described in `message.cpp`:
  void author();
  void content_type();
  void theme();
  void setup_jeff();
  void setup_user();
  void setup_text();
  void setup_markdown();
  void setup_picture();
  // void setup_file();
  void setup_warning();
  void setup_error();
  void prepare_to_widget();
  QPair<QSpacerItem *, Board *> make_layout();
  QString from_plain_to_markdown(QString content);
  QString optimal_line(const QString &remaining, QTextDocument &document, int max_width);
  QString optimal_line(const QString &remaining, int max_sym_width);
};

#endif
