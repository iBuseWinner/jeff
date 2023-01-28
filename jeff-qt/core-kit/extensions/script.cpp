#include "script.hpp"

/*! @brief The constructors. */
ScriptMeta::ScriptMeta() {}
ScriptMeta::ScriptMeta(const QJsonObject &json_object) {
  if (not json_object.contains("stype")) return;
  int _stype = json_object["stype"].toInt();
  if ((_stype > 3) or (_stype < 0)) return;
  stype = ScriptType(_stype);
  if (not (
    json_object.contains("filepath") and
    json_object.contains("fn_name")
  ) and not (
    json_object.contains("program")
  )) return;
  if (json_object.contains("filepath") and json_object.contains("fn_name")) {
    /*! Script that can be ran with embedded Python interpreter. @sa PythonWorker */
    filepath = json_object["filepath"].toString();
    fn_name = json_object["fn_name"].toString();
  } else {
    /*! Some program. @sa DaemonProcess */
    is_for_embedded_python = false;
    if (json_object.contains("working_dir"))
      working_dir = json_object["working_dir"].toString();
    program = json_object["program"].toString();
    if (json_object["envs"].isObject()) {
      auto envs_obj = json_object["envs"].toObject();
      for (auto key : envs_obj.keys()) {
        envs[key] = envs_obj[key].toString();
      }
    }
    if (json_object["args"].isArray()) {
      auto args_arr = json_object["args"].toArray();
      for (auto arg : args_arr) args.append(arg.toString());
    }
  }
  /*! Another options. */
  if (json_object["required_memory_cells"].isArray()) {
    QJsonArray array = json_object["required_memory_cells"].toArray();
    for (auto key : array) required_memory_cells.append(key.toString());
  }
  if (json_object.contains("required_user_input"))
    required_user_input = json_object["required_user_input"].toBool();
  if (json_object.contains("required_history_parts")) {
    required_history_parts = json_object["required_history_parts"].toInt();
    if (required_history_parts < 0) required_history_parts = 0;
  }
  if (json_object.contains("required_adprops"))
    required_adprops = json_object["required_adprops"].toBool();
  valid = true;
}

/*! @brief Turns a script metadata into a JSON object. */
QJsonObject ScriptMeta::to_json() const {
  QJsonObject result = {{"stype", int(stype)}};
  if (is_for_embedded_python) {
    result["filepath"] = filepath;
    result["fn_name"] = fn_name;
  } else {
    result["program"] = program;
    QJsonObject envs_obj;
    for (auto key : envs.keys()) envs_obj[key] = envs[key];
    result["envs"] = envs_obj;
    QJsonArray args_arr;
    for (auto arg : args) args_arr.append(arg);
    result["args"] = args_arr;
  }
  if (required_memory_cells.length() > 0) {
    QJsonArray mcells_arr;
    for (auto cell : required_memory_cells) mcells_arr.append(cell);
    result["required_memory_cells"] = mcells_arr;
  }
  if (required_user_input)
    result["required_user_input"] = required_user_input;
  if (required_history_parts > 0)
    result["required_history_parts"] = required_history_parts;
  if (required_adprops)
    result["required_adprops"] = required_adprops;
  return result;
}

/*! @brief Turns a script metadata into a string. */
QString ScriptMeta::to_string() const {
  QJsonDocument document(to_json());
  return QString::fromUtf8(document.toJson(QJsonDocument::Compact));
}

/*! @brief Translates @a string into a script metadata. */
ScriptMeta *ScriptMeta::from_string(QString string) {
  QJsonParseError errors;
  QJsonDocument document = QJsonDocument::fromJson(string.toUtf8(), &errors);
  if (errors.error != QJsonParseError::NoError) return nullptr;
  auto json_object = document.object();
  auto *script_meta = new ScriptMeta(json_object);
  if (not script_meta) return nullptr;
  if (not script_meta->valid) {
    delete script_meta;
    return nullptr;
  }
  return script_meta;
}
