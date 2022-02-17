#include "jeff.h"

/*! @brief The constructor. */
Jeff::Jeff(int argc, char *argv[]) : QObject() {
  locale = setlocale(LC_ALL, "");
  /*! If settings file does not exist, sets default settings. */
  if (not basis->exists() or not basis->correct()) {
    basis->write(basis->isGreetingsEnabledSt, true);
    basis->write(basis->greetingsMsg, tr("Hello!"));
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
      QTimer::singleShot(2000, this, [this] {
        std::cout << tr("Answer not found.").toStdString() << std::endl;
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
  initscr();
  raw();
  keypad(stdscr, TRUE);
  noecho();
  while (true) {
    int h, w;
    getmaxyx(stdscr, h, w);
    wborder(stdscr, 0, 0, 0, 0, 0, 0, 0, 0);
    mvwprintw(stdscr, 0, 2, "%s", tr("Jeff").toStdString().c_str());
    int y0, x0;
    getyx(stdscr, y0, x0);
    mvwprintw(stdscr, 0, x0 + 2, "%s",
              tr("Enter /q to quit or press Enter to update").toStdString().c_str());
    char *filler = new char[w - 1];
    for (int i = 0; i < w - 2; i++) { filler[i] = ' '; }
    filler[w - 2] = '\0';
    for (int i = 1; i < h - 1; i++) { mvwprintw(stdscr, i, 1, "%s", filler); }
    delete filler;
    auto l = messages.length();
    for (int i = 0; i < l; i++) {
      mvwprintw(stdscr, h - 3 - i, 1, "%s", messages[l - i - 1].toStdString().c_str());
    }
    mvwprintw(stdscr, h - 2, 1, "%s", ">>> ");
    refresh();
    int y1, x1;
    getyx(stdscr, y1, x1);
    QString user_message = ncurses_getstr(y1, x1, w - x1 - 2);
    if (user_message == "/q") {
      qt_shutdown();
      break;
    }
    else { QCoreApplication::instance()->processEvents(); }
    emit send(user_message);
  }
  endwin();
}

/*! @brief Reads a string, correctly handling UTF-8 characters. */
QString Jeff::ncurses_getstr(int y, int x, int available_space) {
  char *filler = new char[available_space];
  for (int i = 0; i < available_space - 1; i++) { filler[i] = ' '; }
  filler[available_space - 1] = '\0';
  std::wstring input;
  wint_t ch;
  while (true) {
    get_wch(&ch);
    if (input.size() >= available_space) break;
    if ((ch == KEY_BACKSPACE) or (ch == 127) or (ch == 8)) {
      if (input.size()) input.pop_back();
      mvwprintw(stdscr, y, x, "%s", filler);
      mvwprintw(stdscr, y, x, "%S", input.c_str());
      refresh();
    } else if (std::iswprint(ch)) {
      input.push_back(ch);
      mvwprintw(stdscr, y, x, "%s", filler);
      mvwprintw(stdscr, y, x, "%S", input.c_str());
      refresh();
    } else if ((ch == KEY_ENTER) or (ch == '\n') or (ch == '\r')) {
      break;
    }
    flushinp();
  }
  delete filler;
  return QString::fromStdWString(input);
}

/*! @brief Adds several messages to the screen. */
void Jeff::start_by(Messages _messages) {
  for (auto message : _messages) add_message_by_md(message);
}

/*! @brief Adds a message to the screen (interactive mode). */
void Jeff::add_message_by_md(MessageData message) {
  if (message.author == Author::Jeff) messages.push_back(tr("Jeff") + ": " + message.content);
  else messages.push_back(tr("You") + ": " + message.content);
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
void Jeff::handle_once(MessageData message) {
  once_mutex.lock();
  if (not once) {
    once = true;
    once_mutex.unlock();
    return;
  }
  std::cout << message.content.toStdString() << std::endl;
  once_mutex.unlock();
  qt_shutdown();
}
