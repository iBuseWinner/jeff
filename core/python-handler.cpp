#include "python-handler.h"

PythonHandler::PythonHandler(QObject *parent) : QObject(parent)
{
  Py_Initialize();
}
