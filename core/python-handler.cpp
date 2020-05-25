#include "python-handler.h"

pythonHandler::pythonHandler(QObject *parent) : QObject(parent)
{
  Py_Initialize();
}
