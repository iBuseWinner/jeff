#ifndef SCRIPT_H
#define SCRIPT_H

/*!
 * @class ScriptMetadata
 * @brief Contains metadata about a script.
 */
class ScriptMetadata {
public:
  /*! Constructors. */
  ScriptMetadata() {}
  ScriptMetadata(const QJsonObject &json_object) {
    path = json_object["path"].toString();
    startup = json_object["startup"].toBool();
    custom_scan = json_object["custom_scan"].toBool();
    answer = json_object["answer"].toBool();
    dynamic_status = json_object["dynamic_status"].toBool();
  }
  /*! The path to the script on disk. */
  QString path;
  /*! Does it have a function that should run every time Jeff starts? */
  bool startup;
  /*! Does it have a function that processes all user input and performs certain
   * actions? */
  bool custom_scan;
  /*! Does it have a function that is performed only when a certain semantic
   * load is found? */
  bool answer;
  /*! Does it have a function that constantly updates the text of the message?
   */
  bool dynamic_status;
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
