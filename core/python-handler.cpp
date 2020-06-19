#include "python-handler.h"

/*!
 * @fn PythonHandler::PythonHandler
 * @brief The constructor.
 * @param parent QObject parent
 */
PythonHandler::PythonHandler(QObject *parent) : QObject(parent) {
  Py_Initialize();
}
