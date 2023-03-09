#ifndef EXT_MANAGER_H
#define EXT_MANAGER_H

#include "core-kit/basis.hpp"
#include "core-kit/history-processor.hpp"
#include "core-kit/notify-client.hpp"
#include "core-kit/database/json.hpp"
#include "core-kit/model/nlp/expression.hpp"
#include "core-kit/model/keystore.hpp"
#include "core-kit/model/message.hpp"
#include "core-kit/model/nlp/cache.hpp"
#include "core-kit/extensions/daemon-process.hpp"
#include "core-kit/extensions/script.hpp"
#include "yelloger.h"
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QList>
#include <QObject>
#include <QPair>
#include <QString>
#include <QVariantMap>

/*! @typedef RunningExtensions
 *  @brief List of working daemons.  */
typedef QList<DaemonProcess *> RunningExtensions;

/*! @class ExtensionsManager
 *  @brief Contains methods of working with extensions.  */
class ExtensionsManager : public QObject {
  Q_OBJECT
  Q_DISABLE_COPY(ExtensionsManager)
public:
  // Functions described in `extensions-manager.cpp`:
  ExtensionsManager(HProcessor *_hp, Basis *_basis, NotifyClient *_notifier, QObject *parent = nullptr);
  ~ExtensionsManager();
  void add_extension(ExtensionMeta *extension_meta);
  void start_extension(ExtensionMeta *extension_meta);
  void remove_extension(ExtensionMeta *extension_meta);
  void stop_extension(ExtensionMeta *extension_meta);
  bool is_running(ExtensionMeta *extension_meta);
  ExtensionsMeta get_extensions_meta();
  void startup();
  void shutdown_extensions();
  QByteArray get_stdout(ExtensionMeta *extension_meta);
  QByteArray get_stderr(ExtensionMeta *extension_meta);
  ExtensionMeta *get_ext_meta_by_name(QString name);

signals:
  /*! @brief Notifies of a extension's error. */
  QString extension_exception(QString error);
  
private:
  // Objects:
  HProcessor *hp = nullptr;
  Basis *basis = nullptr;
  NotifyClient *notifier = nullptr;
  ExtensionsMeta _extensions_meta;
  RunningExtensions _running;
  QString _current_path;
  
  // Functions described in `extensions-manager`:
  void write_exts_in_memory();
};

#endif
