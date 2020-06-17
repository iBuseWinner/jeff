#ifndef PYTHONHANDLER_H
#define PYTHONHANDLER_H

#include <Python.h>
#include <QObject>

/*!
 * Class: PythonHandler.
 * Contains methods of working with Python scripts.
 */
class PythonHandler : public QObject
{
  Q_OBJECT
public:
  /*! Class initialization. */
  PythonHandler(QObject *parent);
};

#endif // PYTHONHANDLER_H
