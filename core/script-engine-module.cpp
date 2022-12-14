#include "script-engine-module.h"

/*! @brief The constructor. */
SEModule::SEModule(HProcessor *_hp, Basis *_basis, NotifyClient *_notifier, QObject *parent)
    : QObject(parent), hp(_hp), basis(_basis), notifier(_notifier) {
  _scripts = basis->json->read_scripts();
}

/*! @brief The destructor. */
SEModule::~SEModule() {
  for (auto *daemon : _daemons) daemon->kill();
  basis->json->write_scripts(_scripts);
  for (auto *script : _scripts) delete script;
  _scripts.clear();
  notifier->unsubscribe_all();
}

/*! @brief Runs functions in scripts intended to start when Jeff starts. */
void SEModule::startup() {
  for (auto *script : _scripts) {
    if (script->stype == ScriptType::Daemon) {
      auto *s = dynamic_cast<DaemonScript *>(script);
      auto *proc = new QProcess(this);
      connect(proc, &QProcess::errorOccurred, this, [this, s]() {
        emit script_exception(
          tr("An error occurred during script execution") + " (" + s->path + ")"
        );
      });
      QFileInfo output(s->path);
      proc->setWorkingDirectory(output.path());
      proc->start(QString("python"), QStringList(s->path));
      _daemons.append(proc);
    } else if (script->stype == ScriptType::Server) {
      auto *s = dynamic_cast<ServerScript *>(script);
      auto *proc = new QProcess(this);
      connect(proc, &QProcess::errorOccurred, this, [this, s]() {
        emit script_exception(
          tr("An error occurred during script execution") + " (" + s->path + ")"
        );
      });
      QFileInfo output(s->path);
      proc->setWorkingDirectory(output.path());
      proc->start(QString("python"), QStringList(s->path));
      _daemons.append(proc);
    } else continue;
  }
}

/*! @brief Adds a script to the general list.
 *  @warning When the program ends, this module will delete the scripts from memory itself.
 *  @sa ~SEModule()  */
void SEModule::add_script(ScriptMetadata *script) { _scripts.append(script); }

/*! @brief Removes a script from the general list. */
bool SEModule::remove_script(ScriptMetadata *script) { return _scripts.removeOne(script); }
/*! @brief Returns the general list of scripts. */
Scripts SEModule::get_scripts() { return _scripts; }
