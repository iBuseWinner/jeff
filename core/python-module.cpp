#include "python-module.h"

/*!
 * @fn PythonModule::PythonModule
 * @brief The constructor.
 */
PythonModule::PythonModule(HProcessor *_hp, Basis *_basis, QObject *parent) : QObject(parent) {
  hp = _hp;
  basis = _basis;
  _scripts = basis->json->read_scripts();
  fill_modules_data();
  Py_Initialize();
  _current_path = QDir::toNativeSeparators(QDir::currentPath());
  QString command = "import sys; sys.path.append('" + _current_path + "')";
  PyRun_SimpleString(command.toStdString().c_str());
}

/*!
 * @fn PythonModule::~PythonModule
 * @brief The destructor.
 */
PythonModule::~PythonModule() {
  basis->json->write_scripts(_scripts);
  Py_Finalize();
}

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

/*!
 * @fn PythonModule::startup
 * @brief Runs functions in scripts intended to start when Jeff starts.
 */
void PythonModule::startup() {
  for (auto script : _scripts)
    if (script.startup) run(script.path, startup_name, QJsonObject::fromVariantMap(QVariantMap()));
}

/*!
 * @fn PythonModule::run
 * @brief Runs a function with parameters and returns the result.
 * @param[in] path to the script
 * @param[in] def_name of function in a script
 * @param[in] args to the function
 * @returns result of @a def_name 's execution
 */
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
    return {{"error_type", 1}};
  }
  Object answer_func = PyObject_GetAttrString(mod, def_name.toStdString().c_str());
  if (not answer_func) {
    emit script_exception(tr("Could not find \"answer\" attribute in module."));
    return {{"error_type", 2}};
  }
  if (not (answer_func && PyCallable_Check(answer_func))) {
    emit script_exception(tr("Cannot call \"answer\": this is not a function!"));
    return {{"error_type", 3}};
  }
  QJsonDocument send_doc(transport);
  Object arg = PyUnicode_FromString(send_doc.toJson(QJsonDocument::Compact).constData());
  if (not arg) {
    emit script_exception(tr("Failed to construct argument from string."));
    return {{"error_type", 4}};
  }
  /*! If we pack in tuple our beautiful Object instead of PyObject, a segmentation fault will */
  Object args = PyTuple_Pack(1, arg.pure());    /*!< come out. Instead we send pure PyObject. */
  if (not args) {
    emit script_exception(tr("Failed to construct a tuple from a string."));
    return {{"error_type", 5}};
  }
  Object answer_result = PyObject_CallObject(answer_func, args);
  if (not answer_result) {
    emit script_exception(tr(
      "The function could not execute correctly because it did not return a result."
    ));
    return {{"error_type", 6}};
  }
  Object objects_representation = PyObject_Repr(answer_result);
  auto result = QString::fromUtf8(PyUnicode_AsUTF8(objects_representation)).chopped(1).remove(0, 1);
  auto recv_doc = QJsonDocument::fromJson(result.toUtf8());
  if (recv_doc.isNull()) {
    emit script_exception(tr("No valid JSON received."));
    return {{"error_type", 7}};
  }
  if (not recv_doc.isObject()) {
    emit script_exception(tr("No JSON object received."));
    return {{"error_type", 8}};
  }
  return recv_doc.object();
}

/*!
 * 
 */
QJsonObject PythonModule::request_answer(Expression &expression) {
  auto doc = QJsonDocument::fromJson(expression.reagent_text.toUtf8());
  if (doc.isNull()) {
    emit script_exception(tr("No valid JSON received."));
    return {{"error_type", 7}};
  }
  if (not doc.isObject()) {
    emit script_exception(tr("No JSON object received."));
    return {{"error_type", 8}};
  }
  auto requirements = doc.object();
  auto transport = QJsonObject();
  if (requirements.contains("get_hist")) {
    int number = requirements["get_hist"].toInt(0);
    if (number > 0) {
      transport["recent"] = QJsonArray();
      auto history = hp->recent(number);
      for (auto msg : history) {
        transport["recent"].toArray()
                           .append(QString("%1: %2")
                           .arg(msg.author == Author::User ? "User" : "Jeff")
                           .arg(msg.content));
      }
    }
  }
  if (requirements.contains("need_values")) {
    QJsonArray val_keys = requirements["need_values"].toArray();
    if (not val_keys.isEmpty()) {
      transport["dict"] = QJsonObject();
      for (auto key : val_keys) {
        QString k = key.toString();
        if (_memory_map.contains(k)) transport["dict"].toObject()[k] = _memory_map[k];
      }
    }
  }
  // Let's find out module path and function's name:
  if (not requirements.contains("script_path")) {
    emit script_exception(tr("The path to the module was not received."));
    return {{"error_type", 9}};
  }
  QString path = requirements["script_path"].toString();
  if (not path.length()) {
    emit script_exception(tr("The path to the module is empty."));
    return {{"error_type", 10}};
  }
  if (not requirements.contains("func_name")) {
    emit script_exception(tr("The name of the function was not received."));
    return {{"error_type", 11}};
  }
  QString func = requirements["func_name"].toString();
  if (not func.length()) {
    emit script_exception(tr("The function name is empty."));
    return {{"error_type", 12}};
  }
  QJsonObject result = run(path, func, transport);
  return result;
}
