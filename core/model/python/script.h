#ifndef SCRIPT_H
#define SCRIPT_H

#include <QMap>

/*! @enum ScriptActions
 *  @brief TBD  */
enum ScriptActions { NoAction, React, Startup, CustomScan, CustomCompose };

/*! @class ScriptMetadata
 *  @brief Contains metadata about a script.  */
class ScriptMetadata {
public:
  /*! Constructors. */
  ScriptMetadata() {}
  ScriptMetadata(const QJsonObject &json_object) {
    path = json_object["path"].toString();
    action = ScriptActions(json_object["action"].toInt());
    fn_name = json_object["fn_name"].toString();
  }
  /*! The path to the script on disk. */
  QString path;
  /*! TBD */
  ScriptActions action;
  /*! TBD */
  QString fn_name;
  
  /*! @brief Turns @a script into a JSON object. */
  QJsonObject to_json() {
    return {
      {"path", path},
      {"action", int(action)},
      {"fn_name", fn_name}
    };
  }
};

/*! @typedef Scripts
 *  @brief Contains metadata about multiple scripts.  */
typedef QList<ScriptMetadata> Scripts;

#endif
