#include "script.h"

/*! @brief Translates @a json_object into a script. */
ScriptMetadata *ScriptsCast::to_script(const QJsonObject &json_object)  {
  ScriptType stype = ScriptType(json_object["stype"].toInt());
  if (stype == ScriptType::React) {
    auto *script = new ReactScript(json_object);
    return script;
  } else if (stype == ScriptType::CustomScan) {
    auto *script = new CustomScanScript(json_object);
    return script;
  } else if (stype == ScriptType::CustomCompose) {
    auto *script = new CustomComposeScript(json_object);
    return script;
  } else return nullptr;
}

DaemonizeableScriptMetadata *ScriptsCast::to_daemon(const QJsonObject &json_object)  {
  ScriptType stype = ScriptType(json_object["stype"].toInt());
  if (stype == ScriptType::Daemon) {
    auto *script = new DaemonScript(json_object);
    return script;
  } else if (stype == ScriptType::Server) {
    auto *script = new ServerScript(json_object);
    return script;
  } else if (stype == ScriptType::Scenario) {
    auto *script = new ScenarioScript(json_object);
    return script;
  } else return nullptr;
}

/*! @brief Translates @a expression into a script. */
ScriptMetadata *ScriptsCast::to_script(QString expression) {
  QJsonParseError errors;
  QJsonDocument document = QJsonDocument::fromJson(expression.toUtf8(), &errors);
  if (errors.error != QJsonParseError::NoError) return nullptr;
  auto json_object = document.object();
  return ScriptsCast::to_script(json_object);
}

/*! @brief Translates @a expression into a script. */
DaemonizeableScriptMetadata *ScriptsCast::to_daemon(QString expression) {
  QJsonParseError errors;
  QJsonDocument document = QJsonDocument::fromJson(expression.toUtf8(), &errors);
  if (errors.error != QJsonParseError::NoError) return nullptr;
  auto json_object = document.object();
  return ScriptsCast::to_daemon(json_object);
}

/*! @brief Turns @a script into a JSON object. */
QJsonObject ScriptsCast::to_json(ScriptMetadata *script) {
  auto stype = script->stype;
  if (stype == ScriptType::React) {
    auto *s = dynamic_cast<ReactScript *>(script);
    return s->to_json();
  } else if (stype == ScriptType::CustomScan) {
    auto *s = dynamic_cast<CustomScanScript *>(script);
    return s->to_json();
  } else if (stype == ScriptType::CustomCompose) {
    auto *s = dynamic_cast<CustomComposeScript *>(script);
    return s->to_json();
  } else return QJsonObject();
}

/*! @brief Turns @a script into a string. */
QString ScriptsCast::to_string(ScriptMetadata *script) {
  auto object = ScriptsCast::to_json(script);
  QJsonDocument document(object);
  return QString::fromUtf8(document.toJson(QJsonDocument::Compact));
}

/*! @brief Turns @a script into a JSON object. */
QJsonObject ScriptsCast::to_json(DaemonizeableScriptMetadata *script) {
  auto stype = script->stype;
  if (stype == ScriptType::Daemon) {
    auto *s = dynamic_cast<DaemonScript *>(script);
    return s->to_json();
  } else if (stype == ScriptType::Server) {
    auto *s = dynamic_cast<ServerScript *>(script);
    return s->to_json();
  } else if (stype == ScriptType::Scenario) {
    auto *s = dynamic_cast<ScenarioScript *>(script);
    return s->to_json();
  } else return QJsonObject();
}

/*! @brief Turns @a script into a string. */
QString ScriptsCast::to_string(DaemonizeableScriptMetadata *script) {
  auto object = ScriptsCast::to_json(script);
  QJsonDocument document(object);
  return QString::fromUtf8(document.toJson(QJsonDocument::Compact));
}
