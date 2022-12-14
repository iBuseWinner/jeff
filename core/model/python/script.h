#ifndef SCRIPT_H
#define SCRIPT_H

#include <QHostAddress>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QMap>

/*! @enum ScriptType
 *  @brief Specifies the types of scripts that are used in Jeff.
 *  @details There are the following types of scripts:
 *  1. React - Reacts to user expressions, added directly to databases
 *  2. Daemon - Run as daemons and interact with Jeff's server as needed
 *  3. Server - Run as daemons, receive notifications of new messages and interact with the server
 *  4. CustomScan - Scans and responds to user input, replacing Jeff's NLPmodule
 *  5. CustomCompose - Receives response options for user input from Jeff and composes the response  */
enum ScriptType { NoAction, React, Daemon, Server, CustomScan, CustomCompose, Scenery };

/*! @class ScriptMetadata
 *  @brief Contains metadata about a script.  */
class ScriptMetadata {
public:
  /*! Constructors. */
  ScriptMetadata() {}
  ScriptMetadata(const QJsonObject &json_object) {
    path = json_object["path"].toString();
    stype = ScriptType(json_object["stype"].toInt());
  }
  /*! Script path. */
  QString path;
  /*! Script type. */
  ScriptType stype = ScriptType::NoAction;
  
  /*! @brief Turns @a script into a JSON object. */
  virtual QJsonObject to_json() {
    return {
      {"path", path},
      {"stype", int(stype)}
    };
  }
};

/*! @brief Container for scripts of different types. */
typedef QList<ScriptMetadata *> Scripts;

/*! @class ReactScript
 *  @brief Contains metadata about reagent script.  */
class ReactScript : public ScriptMetadata {
public:
  /*! Constructors. */
  ReactScript() : ScriptMetadata() {
    stype = ScriptType::React;
  }
  ReactScript(const QJsonObject &json_object) : ScriptMetadata(json_object) {
    if (json_object["memory_cells"].isArray()) {
      QJsonArray array = json_object["memory_cells"].toArray();
      for (auto key : array) memory_cells.append(key.toString());
    }
    hist_parts = json_object["hist_parts"].toInt();
    needs_user_input = json_object["needs_user_input"].toBool();
    fn_name = json_object["fn_name"].toString();
    stype = ScriptType::React;
  }
  /*! List of memory cells that will be passed to the script. */
  QStringList memory_cells;
  /*! The number of previous messages that will be passed to the script to understand the context. */
  int hist_parts = 0;
  /*! Name of function inside the script. */
  QString fn_name;
  /*! Indicates whether the script needs the entire text of the message entered by the user. */
  bool needs_user_input;
  
  /*! @brief Turns @a script into a JSON object. */
  QJsonObject to_json() {
    QJsonArray memory_cells_array;
    for (auto str : memory_cells) memory_cells_array.append(str);
    return {
      {"path", path},
      {"stype", int(stype)},
      {"fn_name", fn_name},
      {"memory_cells", memory_cells_array},
      {"hist_parts", hist_parts},
      {"needs_user_input", needs_user_input}
    };
  }
};

/*! @class DaemonScript
 *  @brief Contains metadata about daemon script.  */
class DaemonScript : public ScriptMetadata {
public:
  /*! Constructors. */
  DaemonScript() : ScriptMetadata() {
    stype = ScriptType::Daemon;
  }
  DaemonScript(const QJsonObject &json_object) : ScriptMetadata(json_object) {
    stype = ScriptType::Daemon;
  }
};

/*! @class ServerScript
 *  @brief Contains metadata about server script.  */
class ServerScript : public ScriptMetadata {
public:
  /*! Constructors. */
  ServerScript() : ScriptMetadata() {
    stype = ScriptType::Server;
  }
  ServerScript(const QJsonObject &json_object) : ScriptMetadata(json_object) {
    server_addr = QHostAddress(json_object["server_addr"].toString());
    server_port = quint16(json_object["server_port"].toInt());
    stype = ScriptType::Server;
  }
  /*! Server address. */
  QHostAddress server_addr;
  /*! Server port. */
  quint16 server_port;
  
  /*! @brief Turns @a script into a JSON object. */
  QJsonObject to_json() {
    return {
      {"path", path},
      {"stype", int(stype)},
      {"server_addr", server_addr.toString()},
      {"server_port", int(server_port)}
    };
  }
};

/*! @class CustomScanScript
 *  @brief Contains metadata about custom scanner.  */
class CustomScanScript : public ScriptMetadata {
public:
  /*! Constructors. */
  CustomScanScript() : ScriptMetadata() {
    stype = ScriptType::CustomScan;
  }
  CustomScanScript(const QJsonObject &json_object) : ScriptMetadata(json_object) {
    fn_name = json_object["fn_name"].toString();
    stype = ScriptType::CustomScan;
  }
  /*! Name of function inside the script. */
  QString fn_name;
  
  /*! @brief Turns @a script into a JSON object. */
  QJsonObject to_json() {
    return {
      {"path", path},
      {"stype", int(stype)},
      {"fn_name", fn_name}
    };
  }
};

/*! @class CustomComposeScript
 *  @brief Contains metadata about custom composer.  */
class CustomComposeScript : public ScriptMetadata {
public:
  /*! Constructors. */
  CustomComposeScript() : ScriptMetadata() {
    stype = ScriptType::CustomCompose;
  }
  CustomComposeScript(const QJsonObject &json_object) : ScriptMetadata(json_object) {
    send_adprops = json_object["send_adprops"].toBool();
    fn_name = json_object["fn_name"].toString();
    stype = ScriptType::CustomCompose;
  }
  /*! Whether to send additional properties of selected phrases to the script. */
  bool send_adprops = false;
  /*! Name of function inside the script. */
  QString fn_name;
  
  /*! @brief Turns @a script into a JSON object. */
  QJsonObject to_json() {
    return {
      {"path", path},
      {"stype", int(stype)},
      {"fn_name", fn_name},
      {"send_adprops", send_adprops}
    };
  }
};

/*! @class SceneryScript
 *  @brief Contains metadata about scenery script.  */
class SceneryScript : public ScriptMetadata {
public:
  /*! Constructors. */
  SceneryScript() : ScriptMetadata() {
    stype = ScriptType::Scenery;
  }
  SceneryScript(const QJsonObject &json_object) : ScriptMetadata(json_object) {
    fn_name = json_object["fn_name"].toString();
    if (json_object["memory_cells"].isArray()) {
      QJsonArray array = json_object["memory_cells"].toArray();
      for (auto key : array) memory_cells.append(key.toString());
    }
    stype = ScriptType::Scenery;
  }
  /*! Name of function inside the script. */
  QString fn_name;
  /*! List of memory cells that will be passed to the script. */
  QStringList memory_cells;
  
  /*! @brief Turns @a script into a JSON object. */
  QJsonObject to_json() {
    QJsonArray memory_cells_array;
    for (auto str : memory_cells) memory_cells_array.append(str);
    return {
      {"path", path},
      {"stype", int(stype)},
      {"fn_name", fn_name},
      {"memory_cells", memory_cells_array}
    };
  }
};

/*! @namespace ScriptsCast
 *  @brief Set of methods for casting script types.  */
namespace ScriptsCast {  
  // Functions described in `script.cpp`:
  ScriptMetadata *to_script(const QJsonObject &json_object);
  ScriptMetadata *to_script(QString expression);
  QJsonObject to_json(ScriptMetadata *script);
  QString to_string(ScriptMetadata *script);
}

#endif
