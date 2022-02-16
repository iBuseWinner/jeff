#ifndef JEFF_H
#define JEFF_H

#include "core/core.h"
#include <QCoreApplication>
#include <QString>
#include <QStringList>
#include <QTimer>
#include <ncurses.h>
#include <iostream>

/*!
 * @mainclass Jeff
 * @brief Application class.
 */
class Jeff : public QObject {
  Q_OBJECT
  Q_DISABLE_COPY(Jeff)
public:
  // Functions described in `jeff.cpp`:
  Jeff(int argc, char *argv[]);
  
signals:
  /*! @brief Indicates that the window is complete and ready for user interaction. */
  void ready_state();
  /*! @brief Sends a request to Core. */
  QString send(QString user_input);
  
private:
  // Objects:
  Core *core = new Core(this);
  Basis *basis = core->basis;
  HProcessor *history_processor = core->hp;
  QStringList messages;
  QMutex once_mutex;
  bool once = false;
  
  // Functions described in `jeff.cpp`:
  void draw();
  void start_by(Messages _messages);
  void add_message_by_md(MessageData message);
  void shutdown();
  void handle_once(MessageData message);
};

#endif
