#ifndef SCRIPT_H
#define SCRIPT_H

#include <QMap>

/*! @enum ScriptActions
 *  @brief TBD  */
enum ScriptActions { NoAction, Reagent, Startup, CustomScan, CustomCompose };

/*! @class ScriptMetadata
 *  @brief Contains metadata about a script.  */
class ScriptMetadata {
public:
  /*! Constructors. */
  ScriptMetadata() {}
  ScriptMetadata(const QJsonObject &json_object) {
    path = json_object["path"].toString();
  }
  /*! The path to the script on disk. */
  QString path;
  
  /*!
   * @fn ScriptMetadata::to_json
   * @brief Turns @a script into a JSON object.
   * @param[in] script
   * @returns converted properties of @a script
   */
  QJsonObject to_json() {
    return {{"path", path},
            {"startup", startup},
            {"custom_scan", custom_scan},
            {"answer", answer},
            {"dynamic_status", dynamic_status}};
  }
};

/*!
 * @typedef Scripts
 * @brief Contains metadata about multiple scripts.
 */
typedef QList<ScriptMetadata> Scripts;

#endif
