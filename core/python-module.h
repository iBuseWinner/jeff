#ifndef PYTHONMODULE_H
#define PYTHONMODULE_H

#include "core/basis.h"
#include "core/database/json.h"
#include "core/model/python/script.h"
#include <QList>
#include <QObject>
#include <QString>
#include <boost/python/exec.hpp>
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
  Scripts _scripts;

  // Constants:
  const char *startup_name = "startup";
  const char *custom_scan_name = "custom_scan";
  const char *answer_name = "answer";
  const char *dynamic_status_name = "dynamic_status";
  const char *custom_compose_name = "custom_compose";

  // Functions described in `python-module.cpp`:
  QList<QVariant> run(QString path, QString def_name, QList<QVariant> args);
};

#endif
