#ifndef PYTHONHANDLER_H
#define PYTHONHANDLER_H

#include <Python.h>
#include <QObject>

/*!
 * @class PythonHandler
 * @brief Contains methods of working with Python scripts.
 */
class PythonHandler : public QObject {
  Q_OBJECT
public:
  // Functions described in `python-handler.cpp`:
  PythonHandler(QObject *parent = nullptr);
};

#endif // PYTHONHANDLER_H
