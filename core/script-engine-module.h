#ifndef SE_MODULE_H
#define SE_MODULE_H

#include "core/basis.h"
#include "core/history-processor.h"
#include "core/notify-client.h"
#include "core/database/json.h"
#include "core/model/expression.h"
#include "core/model/keystore.h"
#include "core/model/message.h"
#include "core/model/nlp/cache.h"
#include "core/model/python/script.h"
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QList>
#include <QObject>
#include <QPair>
#include <QProcess>
#include <QString>
#include <QVariantMap>

/*! @typedef Daemons
 *  @brief List of working daemons.  */
typedef QList<QProcess *> Daemons;

/*! @class ScriptEngineModule @aka SEModule
 *  @brief Contains methods of working with scripts.  */
class SEModule : public QObject {
  Q_OBJECT
  Q_DISABLE_COPY(SEModule)
public:
  // Functions described in `python-module.cpp`:
  SEModule(HProcessor *_hp, Basis *_basis, NotifyClient *_notifier, QObject *parent = nullptr);
  ~SEModule();
  void add_script(ScriptMetadata *script);
  bool remove_script(ScriptMetadata *script);
  Scripts get_scripts();
  void startup();
  void shutdown_daemons();

signals:
  /*! @brief Notifies of a script error. */
  QString script_exception(QString error);
  
private:
  // Objects:
  HProcessor *hp = nullptr;
  Basis *basis = nullptr;
  NotifyClient *notifier = nullptr;
  Scripts _scripts;
  Daemons _daemons;
  QString _current_path;

  // Constants:
  const char *startup_name = "startup";
  const char *custom_scan_name = "custom_scan";
  const char *get_status_update_name = "get_status_update";
  const char *custom_compose_name = "custom_compose";
  const char *action_provider_name = "action_provider";
  const char *go_action_name = "go_action";
};

#endif
