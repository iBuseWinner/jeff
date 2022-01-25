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
#include <QString>
#include <QVariantMap>

#pragma push_macro("slots")
#undef slots
#include <Python.h>
#pragma pop_macro("slots")

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
  QJsonObject request_answer(Expression &expression);

signals:
  QString script_exception(QString error);
  QString send(QString message);
  
private:
  Q_DISABLE_COPY(PythonModule)

  // Objects:
  HProcessor *hp = nullptr;
  Basis *basis = nullptr;
  QMutex *memory_mutex = nullptr;
  KeyStore _memory_map;
  Scripts _scripts;
  ModulesData _mdata;
  QString _current_path;

  // Constants:
  const char *startup_name = "startup";
  const char *custom_scan_name = "custom_scan";
  const char *get_status_update_name = "get_status_update";
  const char *custom_compose_name = "custom_compose";
  const char *action_provider_name = "action_provider";
  const char *go_action_name = "go_action";

  // Functions described in `python-module.cpp`:
  void fill_modules_data();
  QJsonObject run(QString path, QString def_name, QJsonObject values);
  void write_into_memory(QString key, QJsonValue value);
  QJsonValue read_from_memory(QString key);
};

#endif
