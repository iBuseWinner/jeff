#include "worker.hpp"

/*! @brief The constructor. */
PythonWorker::PythonWorker(Basis *_basis, HProcessor *_hp, QObject *parent) : QObject(parent), basis(_basis), hp(_hp) {
  Py_InitializeEx(1);
  // Adds current path to sys.path for importing scripts from this directory.
  _current_path = QDir::currentPath();
  QString command = "import sys; sys.path.append('" + _current_path + "')";
  PyRun_SimpleString(command.toStdString().c_str());
}

/*! @brief The destructor. */
PythonWorker::~PythonWorker() { Py_Finalize(); }

/*! @brief Provides data transfer to the script and unpacking data from the script. */
QJsonObject PythonWorker::run(QString path, QString def_name, QJsonObject transport) {
  QFileInfo module_info(path);
  QString dir_path = module_info.canonicalPath();
  if (dir_path != _current_path) {
    QString command = "import sys; sys.path.append('" + dir_path + "')";
    PyRun_SimpleString(command.toStdString().c_str());
  }
  QString module_nick = module_info.fileName().split('.')[0];
  Object module_name = PyUnicode_FromString(module_nick.toStdString().c_str());
  Object mod = PyImport_Import(module_name);
  if (not mod) {
    emit script_exception(tr("Failed to connect the module."));
    return {{Basis::errorTypeWk, 1}};
  }
  Object answer_func = PyObject_GetAttrString(mod, def_name.toStdString().c_str());
  if (not answer_func) {
    emit script_exception(tr("Could not find \"answer\" attribute in module."));
    return {{Basis::errorTypeWk, 2}};
  }
  if (not (answer_func and PyCallable_Check(answer_func))) {
    emit script_exception(tr("Cannot call \"answer\": this is not a function!"));
    return {{Basis::errorTypeWk, 3}};
  }
  QJsonDocument send_doc(transport);
  Object arg = PyUnicode_FromString(send_doc.toJson(QJsonDocument::Compact).constData());
  if (not arg) {
    emit script_exception(tr("Failed to construct argument from string."));
    return {{Basis::errorTypeWk, 4}};
  }
  /*! If we pack in tuple our beautiful Object instead of PyObject, a segmentation fault will  */
  Object args = PyTuple_Pack(1, arg.get());      /*!< come out. Instead we send pure PyObject. */
  if (not args) {
    emit script_exception(tr("Failed to construct a tuple from a string."));
    return {{Basis::errorTypeWk, 5}};
  }
  Object answer_result = PyObject_CallObject(answer_func, args);
  if (not answer_result) {
    emit script_exception(tr(
      "The function could not execute correctly because it did not return a result."
    ));
    return {{Basis::errorTypeWk, 6}};
  }
  Object objects_representation = PyObject_Repr(answer_result);
  auto result = QString::fromUtf8(PyUnicode_AsUTF8(objects_representation)).chopped(1).remove(0, 2);
  auto recv_doc = QJsonDocument::fromJson(result.toUtf8());
  if (recv_doc.isNull()) {
    emit script_exception(tr("No valid JSON received."));
    return {{Basis::errorTypeWk, 7}};
  }
  if (not recv_doc.isObject()) {
    emit script_exception(tr("No JSON object received."));
    return {{Basis::errorTypeWk, 8}};
  }
  return recv_doc.object();
}

/*!
 * @brief Processes the script configuration and gets the necessary information from it.
 * @details Handles next things:
 *   1. `number_of_hist_messages` prop
 *   2. `memory_cells` prop
 *   3. `properties` from @a expression
 *   4. `path` and `fn_name` props
 *   5. `user_expression` prop
 *   6. <!-- script runs -->
 *   7. `store_in_memory` prop from script
 */
QJsonObject PythonWorker::request_answer(
  ScriptMeta *script, const Expression &expression, const QString &user_expression
) {
  QJsonObject transport;
  if (not script->required_memory_cells.isEmpty()) {
    QJsonObject memory_cells;
    for (auto key : script->required_memory_cells) memory_cells[key] = basis->memory(key);
    transport[Basis::memoryValuesWk] = memory_cells;
  }
  if (script->required_history_parts and not (*basis)[Basis::disableMessagesTransmissionSt].toBool()) {
    QJsonArray history_array;
    auto history = hp->recent(script->required_history_parts);
    for (auto msg : history) 
      history_array.append(
        QString("%1: %2").arg(msg->author == Author::User ? "User" : "Jeff").arg(msg->content)
      );
    transport[Basis::recentMessagesWk] = history_array;
  }
  if (script->required_user_input) transport["user_input"] = user_expression;
  if (not expression.properties.isEmpty()) {
    transport[Basis::exprPropsWk] = Phrase::pack_props(expression.properties);
  }
  if (not script->filepath.length()) {
    emit script_exception(tr("The filepath to the module is empty."));
    return {{Basis::errorTypeWk, 10}};
  }
  if (not script->fn_name.length()) {
    emit script_exception(tr("The function name is empty."));
    return {{Basis::errorTypeWk, 12}};
  }
  QJsonObject result = run(script->filepath, script->fn_name, transport);
  basis->handle_from_script(result, true);
  return result;
}

/*! @brief Prepares data for custom scanning. 
 *  @details It should be noted that users add React scripts either on their own or through prepared bundles,
 *  while custom scanners and composers can be downloaded at any time, and they must be functional.
 *  To run a Python program, you need to know the exact path to the file, so in `*.j.json` files you must
 *  specify the relative path to the `*.py` file, because when it hits Jeff, the script saves
 *  the absolute location in origin. @sa PythonWorker::request_compose */
QJsonObject PythonWorker::request_scan(ScriptMeta *script, const Sources &sources, const QString &user_expression) {
  QJsonObject transport;
  transport["user_expression"] = user_expression;
  QJsonArray sources_obj;
  for (auto source : sources) sources_obj.append(source.to_json());
  transport["sources"] = sources_obj;
  return run(QFileInfo(script->origin).absolutePath() + "/" + script->filepath, script->fn_name, transport);
}

/*! @brief Prepares data for custom composing. */
QJsonObject PythonWorker::request_compose(ScriptMeta *script, const QString &user_expression, CoverageCache sorted) {
  QJsonObject transport;
  transport["user_expression"] = user_expression;
  QJsonArray candidates;
  for (auto ec : sorted) {
    QJsonObject candidate;
    QJsonObject indices;
    for (auto first_indice : ec.coverage_indices.keys()) {
      indices[QString::number(first_indice)] = ec.coverage_indices[first_indice];
    }
    candidate["indices"] = indices;
    candidate["reagent_expression"] = ec.expression.to_json();
    candidate["total_POC"] = ec.total_POC;
    candidates.append(candidate);
  }
  transport["candidates"] = candidates;
  return run(QFileInfo(script->origin).absolutePath() + "/" + script->filepath, script->fn_name, transport);
}
