#ifndef PYTHONMODULE_H
#define PYTHONMODULE_H

#include "core/basis.h"
#include "core/database/json.h"
#include "core/model/keystore.h"
#include "core/model/python/modulesdata.h"
#include "core/model/python/script.h"
#include <QList>
#include <QMutex>
#include <QObject>
#include <QString>
#include <QVariantMap>
#pragma push_macro("slots")
#undef slots
#include <boost/python/exec.hpp>
#include <boost/python/extract.hpp>
#include <boost/python/import.hpp>
#include <boost/python/str.hpp>
#pragma pop_macro("slots")
#include <string>

/*!
 * @class PythonModule
 * @brief Contains methods of working with Python scripts.
 */
class PythonModule : public QObject {
  Q_OBJECT
public:
  // Functions described in `python-module.cpp`:
  PythonModule(Basis *_basis);
  ~PythonModule();
  bool add_script(ScriptMetadata script);
  bool remove_script(ScriptMetadata script);
  Scripts get_scripts();
  void startup();

signals:

private:
  Q_DISABLE_COPY(PythonModule)

  // Objects:
  Basis *basis = nullptr;
  QMutex *memory_mutex = nullptr;
  KeyStore _memory_map;
  Scripts _scripts;

  // Constants:
  const char *startup_name = "startup";
  const char *custom_scan_name = "custom_scan";
  const char *answer_name = "answer";
  const char *dynamic_status_name = "dynamic_status";
  const char *custom_compose_name = "custom_compose";

  // Functions described in `python-module.cpp`:
  QJsonObject run(QString path, QString def_name, QJsonObject values);
  void write_into_memory(QVariant key, QVariant value);
  QVariant read_from_memory(QVariant key);
};

#endif
