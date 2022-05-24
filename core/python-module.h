#ifndef PYTHONMODULE_H
#define PYTHONMODULE_H

#include "core/basis.h"
#include "core/history-processor.h"
#include "core/notify-client.h"
#include "core/database/json.h"
#include "core/model/expression.h"
#include "core/model/keystore.h"
#include "core/model/message.h"
#include "core/model/nlp/cache.h"
#include "core/model/python/object.h"
#include "core/model/python/script.h"
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QList>
#include <QMutex>
#include <QObject>
#include <QProcess>
#include <QString>
#include <QVariantMap>
#include <chrono>
#include <future>
#include <thread>

#pragma push_macro("slots")
#undef slots
#include <Python.h>
#pragma pop_macro("slots")

/*! @typedef Daemons
 *  @brief List of processes running in background.  */
typedef QList<QProcess *> Daemons;

/*!
 * @class PythonModule
 * @brief Contains methods of working with Python scripts.
 */
class PythonModule : public QObject {
  Q_OBJECT
  Q_DISABLE_COPY(PythonModule)
public:
  // Functions described in `python-module.cpp`:
  PythonModule(HProcessor *_hp, Basis *_basis, NotifyClient *_notifier, QObject *parent = nullptr);
  ~PythonModule();
  void add_script(ScriptMetadata *script);
  bool remove_script(ScriptMetadata *script);
  Scripts get_scripts();
  void startup();
  QJsonObject request_answer(
    ReactScript *script, const Expression &expression, const QString &user_expression
  );
  QJsonObject request_scan(CustomScanScript *script, const QString &user_expression);
  QJsonObject request_compose(
    CustomComposeScript *script, const QString &user_expression, CacheWithIndices sorted
  );

signals:
  /*! @brief Notifies of a script error. */
  QString script_exception(QString error);
  /*! @brief Notifies of a message from a script that should be shown on the screen. */
  QString send(QString message);
  
private:
  // Objects:
  HProcessor *hp = nullptr;
  Basis *basis = nullptr;
  NotifyClient *notifier = nullptr;
  Scripts _scripts;
  QString _current_path;
  Daemons _daemons;

  // Constants:
  const char *startup_name = "startup";
  const char *custom_scan_name = "custom_scan";
  const char *get_status_update_name = "get_status_update";
  const char *custom_compose_name = "custom_compose";
  const char *action_provider_name = "action_provider";
  const char *go_action_name = "go_action";

  // Functions described in `python-module.cpp`:
  QJsonObject run(QString path, QString def_name, QJsonObject values);
  Object async_runner(Object func, Object args);
};

#endif
