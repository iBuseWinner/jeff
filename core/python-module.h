#ifndef PYTHONMODULE_H
#define PYTHONMODULE_H

#include "core/database/json.h"
#include "core/model/python/script.h"
#include <QList>
#include <QObject>
#include <QString>
#include <boost/python/exec.hpp>

/*!
 * @class PythonModule
 * @brief Contains methods of working with Python scripts.
 */
class PythonModule : public QObject {
  Q_OBJECT
public:
  // Functions described in `python-module.cpp`:
  PythonModule();
  bool add_script(ScriptMetadata script);
  bool remove_script(ScriptMetadata script);
  Scripts get_scripts();

private:
  Q_DISABLE_COPY(PythonModule)

  // Objects:
  Scripts _scripts;

  // Constants:
  const char *startup_name = "startup";
  const char *custom_scan_name = "custom_scan";
  const char *answer_name = "answer";

  // Functions described in `python-module.cpp`:
  QString run(QString path, QString def_name, QString args);
};

#endif
