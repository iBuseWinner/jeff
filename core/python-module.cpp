#include "python-module.h"

/*!
 * @fn PythonModule::PythonModule
 * @brief The constructor.
 */
PythonModule::PythonModule(Basis *_basis) {
  basis = _basis;
  _scripts = basis->json->read_scripts();
}

/*!
 * @fn PythonModule::~PythonModule
 * @brief The destructor.
 */
PythonModule::~PythonModule() { basis->json->write_scripts(_scripts); }

void PythonModule::startup() {
  for (auto script : _scripts) {
    run(script.path, startup_name, QList<QVariant>());
  }
}

/*!
 * @fn PythonModule::run
 * @brief Runs a function with parameters and returns the result.
 * TODO Не реализована передача данных.
 */
QList<QVariant> PythonModule::run(QString path, QString def_name,
                                  QList<QVariant> args) {
  boost::python::object main = boost::python::import("__main__");
  boost::python::object global(main.attr("__dict__"));
  boost::python::object result = boost::python::exec_file(path, global, global);
  boost::python::object fn = global[def_name];
}
