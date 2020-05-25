#ifndef PYTHONHANDLER_H
#define PYTHONHANDLER_H

#include <Python.h>
#include <QObject>

/*!
 * Class: pythonHandler
 * Contains methods of working with Python scripts.
 */
class pythonHandler : public QObject
{
  Q_OBJECT
public:
  /*! Class initialization. */
  pythonHandler(QObject *parent);
};

#endif // PYTHONHANDLER_H
