#include "jeff.h"

/*! @brief The constructor. */
Jeff::Jeff(int argc, char *argv[]) : QObject() {
  setlocale(LC_ALL, QLocale::system().name().toStdString().c_str());
  /*! If settings file does not exist, sets default settings. */
  if (not basis->exists() or not basis->correct()) {
    basis->write(basis->isGreetingsEnabledSt, true);
    basis->write(basis->greetingsMsg, tr("Hello!"));
  }
  connect(this, &Jeff::ready_state, core, &Core::start);
  connect(this, &Jeff::send, core, &Core::got_message_from_user);
  connect(core, &Core::show, this, &Jeff::add_message_by_md);
  connect(core, &Core::changeMenuBarMonologueCheckbox,
          &(menubar->enable_monologue_mode), &QAction::setChecked);
  connect(history_processor, &HProcessor::history_loaded, this, &Jeff::start_by);
  if (argc > 1) {
    if (argv[1] == "-m" and argc == 3) {
      auto message = QString(argv[2]);
      std::cout << message.toStdString() << std::endl;
    }
  }
}
