#ifndef PYTHONMODULE_H
#define PYTHONMODULE_H

#include "core/basis.h"
#include "core/history-processor.h"
#include "core/database/json.h"
#include "core/model/expression.h"
#include "core/model/keystore.h"
#include "core/model/message.h"
#include "core/model/python/modulesdata.h"
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
public:
  // Functions described in `python-module.cpp`:
  PythonModule(HProcessor *_hp, Basis *_basis, QObject *parent = nullptr);
  ~PythonModule();
  bool add_script(ScriptMetadata script);
  bool remove_script(ScriptMetadata script);
  Scripts get_scripts();
  void startup();
  QJsonObject request_answer(ReactScript *script);

signals:
  QString script_exception(QString error);
  QString send(QString message);
  
private:
  Q_DISABLE_COPY(PythonModule)

  // Objects:
  HProcessor *hp = nullptr;
  Basis *basis = nullptr;
  Scripts _scripts;
  ModulesData _mdata;
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
};

#endif
