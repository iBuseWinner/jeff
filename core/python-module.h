#ifndef PYTHONMODULE_H
#define PYTHONMODULE_H

#include <Python.h>
#include <QObject>

/*!
 * @class PythonModule
 * @brief Contains methods of working with Python scripts.
 */
class PythonModule : public QObject {
  Q_OBJECT
public:
  // Functions described in `python-module.cpp`:
  PythonModule(QObject *parent = nullptr);
};

#endif // PYTHONMODULE_H
