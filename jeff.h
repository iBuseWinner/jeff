#ifndef JEFF_H
#define JEFF_H

#include "core/core.h"
#include <QCoreApplication>
#include <QString>
#include <curses.h>
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
  void ready_state();
  QString send(QString user_input);
  
private:
  // Objects:
  Core *core = new Core(this);
  Basis *basis = core->basis;
  HProcessor *history_processor = core->hp;
  
  // Functions described in `jeff.cpp`:
  void start_by(Messages messages);
  void add_message_by_md(MessageData message);
};

#endif
