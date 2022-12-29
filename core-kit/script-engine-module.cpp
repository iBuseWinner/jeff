#include "script-engine-module.h"

/*! @brief The constructor. */
SEModule::SEModule(HProcessor *_hp, Basis *_basis, NotifyClient *_notifier, QObject *parent)
    : QObject(parent), hp(_hp), basis(_basis), notifier(_notifier) {
  _scripts = basis->json->read_scripts();
}

/*! @brief The destructor. */
SEModule::~SEModule() {
  basis->json->write_scripts(_scripts);
  for (auto *script : _scripts) delete script;
  _scripts.clear();
  notifier->unsubscribe_all();
}

/*! @brief Kills daemons' and servers' processes. */
void SEModule::shutdown_daemons() {
  for (auto *daemon : _daemons) daemon->stop();
}

/*! @brief Runs functions in scripts intended to start when Jeff starts. */
void SEModule::startup() {
  for (auto *script : _scripts) start_daemon(script);
}

/*! @brief Adds a script to the general list. */
void SEModule::add_script(ScriptMetadata *script) {
  start_daemon(script);
  _scripts.append(script);
}

/*! @brief Starts a daemon. */
void SEModule::start_daemon(ScriptMetadata *script) {
  auto *proc = new DaemonProcess(script, this);
  connect(proc, &DaemonProcess::script_exception, this, [this](QString error_text) {
    emit script_exception(error_text);
  });
  proc->start();
  _daemons.append(proc);
  if (script->stype == ScriptType::Server) {
    auto *s = dynamic_cast<ServerScript *>(script);
    notifier->subscribe(s);
  }
}

/*! @brief Removes a script from the general list. */
bool SEModule::remove_script(ScriptMetadata *script) {
  for (auto *daemon : _daemons)
    if (daemon->is_spawner(script)) {
      daemon->stop();
      _daemons.removeOne(daemon);
      break;
    }
  return _scripts.removeOne(script);
}
/*! @brief Returns the general list of scripts. */
Scripts SEModule::get_scripts() { return _scripts; }
