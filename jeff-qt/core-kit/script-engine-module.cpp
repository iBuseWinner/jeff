#include "script-engine-module.h"

/*! @brief The constructor. */
SEModule::SEModule(HProcessor *_hp, Basis *_basis, NotifyClient *_notifier, QObject *parent)
    : QObject(parent), hp(_hp), basis(_basis), notifier(_notifier) {
  _daemons_meta = basis->json->read_daemons();
}

/*! @brief The destructor. */
SEModule::~SEModule() {
  basis->json->write_daemons(_daemons_meta);
  for (auto *daemon_meta : _daemons_meta) delete daemon_meta;
  _daemons_meta.clear();
}

/*! @brief Kills daemons' and servers' processes. */
void SEModule::shutdown_daemons() {
  for (auto *daemon : _daemons) daemon->stop();
  notifier->unsubscribe_all();
}

/*! @brief Runs commands from daemons' metadata intended to start when Jeff starts. */
void SEModule::startup() {
  for (auto *daemon_meta : _daemons_meta) start_daemon(daemon_meta);
}

/*! @brief Adds a script to the general list. */
void SEModule::add_daemon(DaemonizeableScriptMetadata *daemon_meta) {
  start_daemon(daemon_meta);
  _daemons_meta.append(daemon_meta);
}

/*! @brief Starts a daemon. */
void SEModule::start_daemon(DaemonizeableScriptMetadata *daemon_meta) {
  auto *proc = new DaemonProcess(daemon_meta, this);
  connect(proc, &DaemonProcess::script_exception, this, [this](QString error_text) {
    emit script_exception(error_text);
  });
  proc->start();
  _daemons.append(proc);
  if (daemon_meta->stype == ScriptType::Server) {
    auto *s = dynamic_cast<ServerScript *>(daemon_meta);
    notifier->subscribe(s);
  }
}

/*! @brief Removes a daemon metadata and stops its daemon from the general list. */
bool SEModule::remove_daemon(DaemonizeableScriptMetadata *daemon_meta) {
  for (auto *daemon : _daemons)
    if (daemon->is_spawner(daemon_meta)) {
      daemon->stop();
      _daemons.removeOne(daemon);
      break;
    }
  if (daemon_meta->stype == ScriptType::Server) {
    auto *s = dynamic_cast<ServerScript *>(daemon_meta);
    notifier->unsubscribe(s);
  }
  return _daemons_meta.removeOne(daemon_meta);
}
/*! @brief Returns the general list of daemons' metadata. */
DaemonsMeta SEModule::get_daemons() { return _daemons_meta; }
