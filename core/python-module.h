#ifndef PYTHONMODULE_H
#define PYTHONMODULE_H

#include "core/database/json.h"
#include <QObject>

#pragma push_macro("slots")
#undef slots
#include "Python.h"
#pragma pop_macro("slots")

/*!
 * @class PythonModule
 * @brief Contains methods of working with Python scripts.
 */
class PythonModule : public QObject {
  Q_OBJECT
public:
  // Functions described in `python-module.cpp`:
  PythonModule(QObject *parent = nullptr);
  ~PythonModule();
  QString run(QString module_path, QString args);
};

#endif
