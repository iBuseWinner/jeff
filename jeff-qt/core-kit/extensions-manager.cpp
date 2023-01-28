#include "extensions-manager.h"

/*! @brief The constructor. */
ExtensionsManager::ExtensionsManager(
  HProcessor *_hp, Basis *_basis, NotifyClient *_notifier, QObject *parent
) : QObject(parent), hp(_hp), basis(_basis), notifier(_notifier) {
  _extensions_meta = basis->json->read_extensions();
}

/*! @brief The destructor. */
ExtensionsManager::~ExtensionsManager() {
  basis->json->write_extensions(_extensions_meta);
  for (auto *extension_meta : _extensions_meta) delete extension_meta;
  for (auto *extension : _running) delete extension;
  _extensions_meta.clear();
  _running.clear();
}

/*! @brief Kills daemons' and servers' processes. */
void ExtensionsManager::shutdown_extensions() {
  for (auto *extension : _running) extension->stop();
  notifier->unsubscribe_all();
}

/*! @brief Runs commands from daemons' metadata intended to start when Jeff starts. */
void ExtensionsManager::startup() {
  for (auto *extension_meta : _extensions_meta) if (extension_meta->enabled) start_extension(extension_meta);
}

/*! @brief Adds an extension to the general list. */
void ExtensionsManager::add_extension(ExtensionMeta *extension_meta) {
  start_extension(extension_meta);
  _extensions_meta.append(extension_meta);
}

/*! @brief Starts the daemon. */
void ExtensionsManager::start_extension(ExtensionMeta *extension_meta) {
  auto *proc = new DaemonProcess(basis, extension_meta, this);
  connect(proc, &DaemonProcess::daemon_exception, this, [this](QString error_text) {
    emit extension_exception(error_text);
  });
  proc->start();
  _running.append(proc);
  if (extension_meta->is_server) notifier->subscribe(extension_meta);
}

/*! @brief Stops the daemon. */
void ExtensionsManager::stop_extension(ExtensionMeta *extension_meta) {
  if (extension_meta->is_server) notifier->unsubscribe(extension_meta);
  for (auto *proc : _running) if (proc->is_spawner(extension_meta)) {
    proc->stop();
    disconnect(proc, &DaemonProcess::daemon_exception, nullptr, nullptr);
    _running.removeOne(proc);
    break;
  }
}

/*! @brief Removes an extension metadata and stops its daemon from the general list. */
void ExtensionsManager::remove_extension(ExtensionMeta *extension_meta) {
  stop_extension(extension_meta);
  _extensions_meta.removeOne(extension_meta);
  delete extension_meta;
}

/*! @brief Returns the state of the extension. */
bool ExtensionsManager::is_running(ExtensionMeta *extension_meta) {
  for (auto *extension : _running)
    if (extension->is_spawner(extension_meta)) {
      if (extension->state() == QProcess::Running) return true;
      else break;
    }
  return false;
}

/*! @brief Returns the general list of daemons' metadata. */
ExtensionsMeta ExtensionsManager::get_extensions_meta() { return _extensions_meta; }
