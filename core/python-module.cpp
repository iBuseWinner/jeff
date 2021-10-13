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
    run(script.path, startup_name, KeyStore());
  }
}

/*!
 * @fn PythonModule::run
 * @brief Runs a function with parameters and returns the result.
 * @param[in] path to the script
 * @param[in] def_name of function in a script
 * @param[in] args to the function
 * @returns result of @a def_name 's execution
 */
KeyStore PythonModule::run(QString path, QString def_name, KeyStore args) {
  boost::python::object main = boost::python::import("__main__");
  boost::python::object global(main.attr("__dict__"));
  boost::python::object result =
      boost::python::exec_file(path.toStdString().c_str(), global, global);
  boost::python::object fn = global[def_name];
  auto json_args = basis->json->to_json(args);
  return basis->json->from_json(boost::python::extract<std::string>(
      fn(boost::python::str(json_args.c_str()))));
}
