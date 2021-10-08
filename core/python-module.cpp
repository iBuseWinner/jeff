#include "python-module.h"

/*!
 * @fn PythonModule::PythonModule
 * @brief The constructor.
 * @param parent QObject parent
 */
PythonModule::PythonModule(QObject *parent) : QObject(parent) {
  Py_Initialize();
}

PythonModule::~PythonModule() {
  Py_Finalize();
}

QString PythonModule::run(QString module_path, QString args) {
  
}
