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

/*!
 * @fn PythonModule::startup
 * @brief Runs functions in scripts intended to start when Jeff starts.
 */
void PythonModule::startup() {
  for (auto script : _scripts)
    if (script.startup)
      run(script.path, startup_name, QJsonObject::fromVariantMap(QVariantMap()));
}

/*!
 * @fn PythonModule::run
 * @brief Runs a function with parameters and returns the result.
 * @param[in] path to the script
 * @param[in] def_name of function in a script
 * @param[in] args to the function
 * @returns result of @a def_name 's execution
 */
QJsonObject PythonModule::run(QString path, QString def_name,
                              QJsonObject args) {
  boost::python::object main = boost::python::import("__main__");
  boost::python::object global(main.attr("__dict__"));
  boost::python::object result =
      boost::python::exec_file(path.toStdString().c_str(), global, global);
  boost::python::object fn = global[def_name];
  return QJsonDocument::fromJson(
             QByteArray::fromStdString(
                 boost::python::extract<std::string>(fn(boost::python::str(
                     QJsonDocument(args).toJson().toStdString().c_str())))))
      .object();
}
