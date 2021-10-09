#ifndef PYTHONMODULE_H
#define PYTHONMODULE_H

#include "core/database/json.h"
#include <QList>
#include <QObject>
#include <QString>

/*!
 * @struct ScriptMetadata
 */
struct ScriptMetadata {
  QString path;
  bool startup;
  bool custom_scan;
  bool answer;
};

/*!
 * @class PythonModule
 * @brief Contains methods of working with Python scripts.
 */
class PythonModule : public QObject {
  Q_OBJECT
public:
  // Functions described in `python-module.cpp`:
  PythonModule();
  QString run(QString module_path, QString def_name, QString args);

private:
  // Objects:
  QList<
};

#endif
