#ifndef JEFF_H
#define JEFF_H

#include "core-kit/core.hpp"
#include <QCoreApplication>
#include <QString>
#include <QStringList>
#include <QTimer>
#include <chrono>
#include <iostream>
#include <locale>
#include <ncurses.h>
#include <thread>

/*! @mainclass Jeff
 *  @brief Application class.  */
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
  std::wstring buffer;
  bool buffer_changed = true;
  QStringList messages;
  QMutex once_mutex;
  bool once = false;
  
  // Functions described in `jeff.cpp`:
  void ncurses_draw();
  bool ncurses_getstr(int y, int x, int available_space);
  void qt_shutdown();
  void start_by(MessagesMeta _messages);
  void add_message_by_md(MessageMeta message);
  void handle_once(MessageMeta message);
};

#endif
