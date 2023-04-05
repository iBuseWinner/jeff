#include "jeff.hpp"

/*! @brief The constructor. */
Jeff::Jeff(int argc, char *argv[]) : QObject() {
  setlocale(LC_ALL, "");
  /*! If settings file does not exist, sets default settings. */
  if (not basis->exists() or not basis->correct()) {
    basis->write(Basis::isGreetingsEnabledSt, true);
    basis->write(Basis::greetingsMsg, tr("Hello!"));
    basis->write(Basis::scenarioExitMsg, "//");
    basis->write(Basis::serverPortSt, 8005);
  }
  connect(this, &Jeff::send, core, &Core::got_message_from_user);
  if (argc > 1) {
    if ((strcmp(argv[1], "-m") == 0) and (argc == 3)) {
      /*! @mode Single shot
       *  @code
       *    $ jeff -m '<your message>'
       *  @endcode  */
      auto message = QString(argv[2]);
      connect(core, &Core::show, this, &Jeff::handle_once);
      emit send(message);
      QTimer::singleShot(0, this, [this] {
        QCoreApplication::instance()->processEvents();
      });
      QTimer::singleShot(5000, this, [this] {
        std::cout << tr("Answer not found.").toStdString() << std::endl;
        core->em->shutdown_extensions();
        qt_shutdown();
      });
    }
  } else {
    /*! @mode Interactive
     *  @code
     *    $ jeff ...
     *  @endcode  */
    connect(this, &Jeff::ready_state, core, &Core::start);
    connect(this, &Jeff::ready_state, this, &Jeff::ncurses_draw);
    connect(core, &Core::show, this, &Jeff::add_message_by_md);
    connect(history_processor, &HProcessor::history_loaded, this, &Jeff::start_by);
  }
}

/*! @brief Draws a console window, asks for an input string, and handles Jeff's events. */
void Jeff::ncurses_draw() {
  using namespace std::chrono_literals;
  initscr();
  raw();
  keypad(stdscr, TRUE);
  nodelay(stdscr, TRUE);
  noecho();
  while (true) {
    int h, w;
    getmaxyx(stdscr, h, w);
    wborder(stdscr, 0, 0, 0, 0, 0, 0, 0, 0);
    mvwprintw(stdscr, 0, 2, "%s", tr("Jeff").toStdString().c_str());
    int y0, x0;
    getyx(stdscr, y0, x0);
    mvwprintw(stdscr, 0, x0 + 2, "%s", tr("Enter /q to quit").toStdString().c_str());
    char *filler = new char[w - 1];
    for (int i = 0; i < w - 2; i++) { filler[i] = ' '; }
    filler[w - 2] = '\0';
    for (int i = 1; i < h - 2; i++) { mvwprintw(stdscr, i, 1, "%s", filler); }
    auto l = messages.length();
    for (int i = 0; i < l; i++) {
      mvwprintw(stdscr, h - 3 - i, 1, "%s", messages[l - i - 1].toStdString().c_str());
    }
    move(h - 2, 5 + buffer.size());
    if (buffer_changed) {
      mvwprintw(stdscr, h - 2, 1, "%s", filler);
      mvwprintw(stdscr, h - 2, 1, "%s", ">>> ");
      wprintw(stdscr, "%S", buffer.c_str());
      buffer_changed = false;
    }
    delete[] filler;
    refresh();
    int y1, x1;
    getyx(stdscr, y1, x1);
    if (ncurses_getstr(y1, x1, w - x1 - 2)) {
      QString user_message = QString::fromStdWString(buffer);
      buffer.clear();
      if (user_message == "/q") {
        core->em->shutdown_extensions();
        qt_shutdown();
        break;
      }
      emit send(user_message);
    }
    QCoreApplication::instance()->processEvents();
    std::this_thread::sleep_for(50ms);
  }
  endwin();
}

/*! @brief Reads a string, correctly handling UTF-8 characters. */
bool Jeff::ncurses_getstr(int y, int x, int available_space) {
  wint_t ch;
  bool res = false;
  get_wch(&ch);
  if (buffer.length() >= available_space) {
    res = true;
    buffer_changed = true;
  }
  else if ((ch == KEY_ENTER) or (ch == '\n') or (ch == '\r')) {
    res = true;
    buffer_changed = true;
  }
  else if ((ch == KEY_BACKSPACE) or (ch == 127) or (ch == 8)) {
    if (buffer.size()) buffer.pop_back();
    buffer_changed = true;
    refresh();
  } else if (std::iswprint(ch)) {
    buffer.push_back(ch);
    buffer_changed = true;
    refresh();
  }
  flushinp();
  return res;
}

/*! @brief Adds several messages to the screen. */
void Jeff::start_by(MessagesMeta _messages) {
  for (auto *message : _messages) add_message_by_md(message);
}

/*! @brief Adds a message to the screen (interactive mode). */
void Jeff::add_message_by_md(MessageMeta *message) {
  if (message->author == Author::Jeff) messages.push_back(tr("Jeff") + ": " + message->content);
  else messages.push_back(tr("You") + ": " + message->content);
  int h, w;
  getmaxyx(stdscr, h, w);
  if (messages.length() + 3 > h) messages.pop_front();
}

/*! @brief Terminates the application (interactive mode). */
void Jeff::qt_shutdown() {
  QTimer::singleShot(0, this, [this] {
    QCoreApplication::instance()->quit();
  });
}

/*! @brief Processes a single message, returns a response, and terminates the application. */
void Jeff::handle_once(MessageMeta *message) {
  once_mutex.lock();
  if (not once) {
    once = true;
    once_mutex.unlock();
    return;
  }
  std::cout << message->content.toStdString() << std::endl;
  once_mutex.unlock();
  qt_shutdown();
}
