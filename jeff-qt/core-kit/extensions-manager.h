#ifndef EXT_MANAGER_H
#define EXT_MANAGER_H

#include "core-kit/basis.h"
#include "core-kit/history-processor.h"
#include "core-kit/notify-client.h"
#include "core-kit/database/json.h"
#include "core-kit/model/nlp/expression.h"
#include "core-kit/model/keystore.h"
#include "core-kit/model/message.h"
#include "core-kit/model/nlp/cache.h"
#include "core-kit/extensions/daemon-process.h"
#include "core-kit/extensions/script.h"
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

/*! @typedef Daemons
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
  void add_extension(ExtensionMeta *extension);
  void remove_extension(ExtensionMeta *extension);
  ExtensionsMeta get_extensions_meta();
  void startup();
  void shutdown_extensions();

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
  
  // Functions described in `extensions-manager.cpp`:
  void start_extension(ExtensionMeta *extension_meta);
};

#endif
