#include "python-module.h"

/*!
 * @fn PythonModule::PythonModule
 * @brief The constructor.
 */
PythonModule::PythonModule() {
  
}

QString PythonModule::run(QString module_path, QString def_name, QString args) {
  boost::python::object main = boost::python::import("__main__");
  boost::python::object global(main.attr("__dict__"));
  boost::python::object result = boost::python::exec_file(module_path, global, global);
  boost::python::object fn = global[def_name];
  
}
