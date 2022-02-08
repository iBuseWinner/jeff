#include "python-module.h"

/*! @brief The constructor. */
PythonModule::PythonModule(HProcessor *_hp, Basis *_basis, QObject *parent)
    : QObject(parent), hp(_hp), basis(_basis) {
  _scripts = basis->json->read_scripts();
  fill_modules_data();
  Py_Initialize();
  _current_path = QDir::toNativeSeparators(QDir::currentPath());
  QString command = "import sys; sys.path.append('" + _current_path + "')";
  PyRun_SimpleString(command.toStdString().c_str());
}

/*! @brief The destructor. */
PythonModule::~PythonModule() { basis->json->write_scripts(_scripts); Py_Finalize(); }

/*! @brief */
void PythonModule::fill_modules_data() {
  for (auto script : _scripts) {
    QFile f(script.path);
    if (not f.open(QIODevice::ReadOnly)) {
      emit script_exception(tr("The module with path ") + script.path + tr(" cannot be opened."));
      continue;
    }
    auto str = QString(f.readAll());
    f.close();
    if (QString(f.readAll()).contains("def startup("))
      _mdata.startup_paths.append(script.path);
    if (QString(f.readAll()).contains("def custom_scan("))
      _mdata.custom_scan_paths.append(script.path);
    if (QString(f.readAll()).contains("def action_provider(")) {
    //  auto vals = run(script.path, 
    }
  }
}

/*! @brief Runs functions in scripts intended to start when Jeff starts. */
void PythonModule::startup() {
  for (auto script : _scripts)
    if (script.startup) run(script.path, startup_name, QJsonObject::fromVariantMap(QVariantMap()));
}

/*! @brief Runs a function with parameters and returns the result. */
QJsonObject PythonModule::run(QString path, QString def_name, QJsonObject transport) {
  QFileInfo module_info(path);
  QString dir_path = QDir::toNativeSeparators(module_info.canonicalPath());
  if (dir_path != _current_path) {
    QString command = "import sys; sys.path.append('" + dir_path + "')";
    PyRun_SimpleString(command.toStdString().c_str());
  }
  QString module_nick = module_info.fileName().split('.')[0];
  Object module_name = PyUnicode_FromString(module_nick.toStdString().c_str());
  Object mod = PyImport_Import(module_name);
  if (not mod) {
    emit script_exception(tr("Failed to connect the module."));
    return {{basis->errorTypeWk, 1}};
  }
  Object answer_func = PyObject_GetAttrString(mod, def_name.toStdString().c_str());
  if (not answer_func) {
    emit script_exception(tr("Could not find \"answer\" attribute in module."));
    return {{basis->errorTypeWk, 2}};
  }
  if (not (answer_func && PyCallable_Check(answer_func))) {
    emit script_exception(tr("Cannot call \"answer\": this is not a function!"));
    return {{basis->errorTypeWk, 3}};
  }
  QJsonDocument send_doc(transport);
  Object arg = PyUnicode_FromString(send_doc.toJson(QJsonDocument::Compact).constData());
  if (not arg) {
    emit script_exception(tr("Failed to construct argument from string."));
    return {{basis->errorTypeWk, 4}};
  }
  /*! If we pack in tuple our beautiful Object instead of PyObject, a segmentation fault will */
  Object args = PyTuple_Pack(1, arg.pure());    /*!< come out. Instead we send pure PyObject. */
  if (not args) {
    emit script_exception(tr("Failed to construct a tuple from a string."));
    return {{basis->errorTypeWk, 5}};
  }
  Object answer_result = PyObject_CallObject(answer_func, args);
  if (not answer_result) {
    emit script_exception(tr(
      "The function could not execute correctly because it did not return a result."
    ));
    return {{basis->errorTypeWk, 6}};
  }
  Object objects_representation = PyObject_Repr(answer_result);
  auto result = QString::fromUtf8(PyUnicode_AsUTF8(objects_representation)).chopped(1).remove(0, 1);
  auto recv_doc = QJsonDocument::fromJson(result.toUtf8());
  if (recv_doc.isNull()) {
    emit script_exception(tr("No valid JSON received."));
    return {{basis->errorTypeWk, 7}};
  }
  if (not recv_doc.isObject()) {
    emit script_exception(tr("No JSON object received."));
    return {{basis->errorTypeWk, 8}};
  }
  return recv_doc.object();
}

/*!
 * @brief Processes the script configuration and gets the necessary information from it.
 * @details Handles next things:
 *   1. `get_hist` prop
 *   2. `need_values` prop
 *   3. `properties` from @a expression
 *   4. `script_path` and `func_name` props
 *   5. <!-- script runs -->
 *   6. `store_values` prop from script
 */
QJsonObject PythonModule::request_answer(Expression &expression) {
  auto doc = QJsonDocument::fromJson(expression.reagent_text.toUtf8());
  if (doc.isNull()) {
    emit script_exception(tr("No valid JSON received."));
    return {{basis->errorTypeWk, 7}};
  }
  if (not doc.isObject()) {
    emit script_exception(tr("No JSON object received."));
    return {{basis->errorTypeWk, 8}};
  }
  auto requirements = doc.object();
  auto transport = basis->handle_to_script(requirements);
  if (requirements.contains(basis->getHistWk)) {
    int number = requirements[basis->getHistWk].toInt(0);
    if (number > 0) {
      transport[basis->recentWk] = QJsonArray();
      auto history = hp->recent(number);
      for (auto msg : history) {
        transport[basis->recentWk].toArray()
                                  .append(QString("%1: %2")
                                  .arg(msg.author == Author::User ? "User" : "Jeff")
                                  .arg(msg.content));
      }
    }
  }
  if (not expression.properties.isEmpty()) {
    transport[basis->exprPropsWk] = Expression::pack_props(expression.properties);
  }
  if (not requirements.contains(basis->scriptPathWk)) {
    emit script_exception(tr("The path to the module was not received."));
    return {{basis->errorTypeWk, 9}};
  }
  QString path = requirements[basis->scriptPathWk].toString();
  if (not path.length()) {
    emit script_exception(tr("The path to the module is empty."));
    return {{basis->errorTypeWk, 10}};
  }
  if (not requirements.contains(basis->funcNameWk)) {
    emit script_exception(tr("The name of the function was not received."));
    return {{basis->errorTypeWk, 11}};
  }
  QString func = requirements[basis->funcNameWk].toString();
  if (not func.length()) {
    emit script_exception(tr("The function name is empty."));
    return {{basis->errorTypeWk, 12}};
  }
  QJsonObject result = run(path, func, transport);
  basis->handle_from_script(result, true);
  return result;
}
