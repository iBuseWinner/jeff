#ifndef MESSAGE_W_H
#define MESSAGE_W_H

#include "core-kit/model/message.hpp"
#include "dialogues/modal-handler.hpp"
#include "lib/hoedown/src/buffer.h"
#include "lib/hoedown/src/document.h"
#include "lib/hoedown/src/html.h"
#include "widgets/board.hpp"
#include "widgets/menu.hpp"
#include "widgets/layouts/grid.hpp"
#include "widgets/layouts/linears.hpp"
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
  /*! @brief Returns @a message_data. */
  MessageMeta *message_data() { return md; }

  // Functions described in `message.cpp`:
  Message();
  Message(MessageMeta *_md);
  void message_data(MessageMeta *_md);
  void widget(ModalHandler *modal_handler);
  void update_text(const QString &text);
  void setWidth(int width);
  void fit_text(int width);

signals:
  /*! @brief When the message is closed, it can inform ModalHandler, and then it will delete the message from Display.  */
  void closed();

private:
  // Objects:
  MessageMeta *md = nullptr;
  GridLt *grid_layout = nullptr;
  QWidget *w = nullptr;
  int _width = 0;
  bool non_ideal_width_completed = false;
  QString precached_md = QString();

  // Constants:
  static const ushort standardMargin = 6;

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
  QString optimal_line(const QString &remaining, const int &start_index, int max_sym_width);
};

#endif
