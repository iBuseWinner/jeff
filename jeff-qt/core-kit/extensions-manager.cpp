#include "extensions-manager.hpp"

/*! @brief The constructor. */
ExtensionsManager::ExtensionsManager(
  HProcessor *_hp, Basis *_basis, NotifyClient *_notifier, QObject *parent
) : QObject(parent), hp(_hp), basis(_basis), notifier(_notifier) {
  _extensions_meta = basis->json->read_extensions();
  write_exts_in_memory();
}

/*! @brief The destructor. */
ExtensionsManager::~ExtensionsManager() {
  Yellog::Trace("ExtensionsManager into destructor:");
  basis->json->write_extensions(_extensions_meta);
  Yellog::Trace("\tWrote extensions.");
  for (auto *extension_meta : _extensions_meta) delete extension_meta;
  for (auto *extension : _running) delete extension;
  Yellog::Trace("\tMemory freed.");
  _extensions_meta.clear();
  _running.clear();
  Yellog::Trace("\tLists cleared.");
}

/*! @brief Kills daemons' and servers' processes. */
void ExtensionsManager::shutdown_extensions() {
  bool ok = true;
  int ms = (*basis)[Basis::extensionKillSecSt].toInt(&ok) * 1000;
  if (not ok) ms = 60000;
  for (auto *extension : _running) extension->stop(ms);
  Yellog::Trace("All extensions have been stopped.");
  notifier->unsubscribe_all();
  Yellog::Trace("All extensions have been unsubscribed from notifier.");
}

/*! @brief Runs commands from daemons' metadata intended to start when Jeff starts. */
void ExtensionsManager::startup() {
  for (auto *extension_meta : _extensions_meta) if (extension_meta->enabled) start_extension(extension_meta);
}

/*! @brief Adds an extension to the general list. */
void ExtensionsManager::add_extension(ExtensionMeta *extension_meta) {
  Yellog::Trace("Got extension %p", extension_meta);
  _extensions_meta.append(extension_meta);
  Yellog::Trace("\tExtension added.");
  start_extension(extension_meta);
  Yellog::Trace("\tExtension started.");
  write_exts_in_memory();
}

/*! @brief Starts the daemon. */
void ExtensionsManager::start_extension(ExtensionMeta *extension_meta) {
  Yellog::Trace("Got extension %p", extension_meta);
  // Removing old process if exists.
  {
    DaemonProcess *old_proc = nullptr;
    for (auto *extension : _running)
      if (extension->is_spawner(extension_meta)) {
        old_proc = extension;
        break;
      }
    if (old_proc) {
      old_proc->disconnect();
      _running.removeOne(old_proc);
    }
  }
  auto *proc = new DaemonProcess(basis, extension_meta, this);
  connect(proc, &DaemonProcess::daemon_exception, this, [this] (QString error_text) {
    emit extension_exception(error_text);
  });
  Yellog::Trace("\tStarting a process...");
  proc->start(QProcess::Unbuffered | QProcess::ReadOnly);
  _running.append(proc);
  if (extension_meta->is_server) {
    Yellog::Trace("\tSubscribing to notifier...");
    notifier->subscribe(extension_meta);
  }
}

/*! @brief Stops the daemon. */
void ExtensionsManager::stop_extension(ExtensionMeta *extension_meta) {
  Yellog::Trace("Got extension %p", extension_meta);
  if (extension_meta->is_server) {
    Yellog::Trace("\tUnsubscribing from notifier...");
    notifier->unsubscribe(extension_meta);
  }
  bool ok = true;
  int ms = (*basis)[Basis::extensionKillSecSt].toInt(&ok) * 1000;
  if (not ok) ms = 60000;
  for (auto *proc : _running) if (proc->is_spawner(extension_meta)) {
    proc->stop(ms);
    Yellog::Trace("\tStopped the process");
    disconnect(proc, &DaemonProcess::daemon_exception, nullptr, nullptr);
    _running.removeOne(proc);
    break;
  }
}

/*! @brief Removes an extension metadata and stops its daemon from the general list. */
void ExtensionsManager::remove_extension(ExtensionMeta *extension_meta) {
  Yellog::Trace("Got extension %p", extension_meta);
  stop_extension(extension_meta);
  _extensions_meta.removeOne(extension_meta);
  Yellog::Trace("\tExtension has been removed from Jeff.");
  delete extension_meta;
  Yellog::Trace("\tExtension memory has been freed.");
  write_exts_in_memory();
}

/*! @brief Returns the state of the extension. */
bool ExtensionsManager::is_running(ExtensionMeta *extension_meta) {
  Yellog::Trace("Got extension %p", extension_meta);
  for (auto *extension : _running)
    if (extension->is_spawner(extension_meta)) {
      Yellog::Trace("\tExtension running status: %d", ((extension->state() == QProcess::Running) ? 1 : 0));
      if (extension->state() == QProcess::Running) return true;
      else break;
    }
  return false;
}

/*! @brief Returns the general list of daemons' metadata. */
ExtensionsMeta ExtensionsManager::get_extensions_meta() { return _extensions_meta; }

/*! @brief Returns the output of the extension. */
QByteArray ExtensionsManager::get_stdout(ExtensionMeta *extension_meta) {
  for (auto *extension : _running)
    if (extension->is_spawner(extension_meta))
      return extension->get_output();
  return QByteArray();
}

/*! @brief Returns the errors of the extension. */
QByteArray ExtensionsManager::get_stderr(ExtensionMeta *extension_meta) {
  for (auto *extension : _running)
    if (extension->is_spawner(extension_meta))
      return extension->get_error();
  return QByteArray();
}

/*! @brief Looks for an extension by name. */
ExtensionMeta *ExtensionsManager::get_ext_meta_by_name(QString name) {
  for (auto *extension_meta : _extensions_meta)
    if (name == extension_meta->name)
      return extension_meta;
  return nullptr;
}

/*! @brief TBD */
void ExtensionsManager::write_exts_in_memory() {
  QJsonArray arr;
  for (auto *ext_meta : _extensions_meta) arr.append(ext_meta->name);
  basis->memory("jeff-extensions-list", arr);
}
