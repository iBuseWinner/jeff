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
enum ScriptType { NoAction, React, Daemon, Server, CustomScan, CustomCompose, Scenario };

/*! @class ScriptMetadata
 *  @brief Contains metadata about a script.  */
class ScriptMetadata {
public:
  /*! Constructors. */
  ScriptMetadata() {}
  ScriptMetadata(const QJsonObject &json_object) {
    stype = ScriptType(json_object["stype"].toInt());
  }
  /*! Script type. */
  ScriptType stype = ScriptType::NoAction;
  
  /*! @brief Turns @a script into a JSON object. */
  virtual QJsonObject to_json() {
    return {{"stype", int(stype)}};
  }
};

/*! @brief Container for scripts of different types. */
typedef QList<ScriptMetadata *> ScriptsMeta;

/*! @class ReactScript
 *  @brief Contains metadata about reagent script.  */
class ReactScript : public ScriptMetadata {
public:
  /*! Constructors. */
  ReactScript() : ScriptMetadata() {
    stype = ScriptType::React;
  }
  ReactScript(const QJsonObject &json_object) : ScriptMetadata(json_object) {
    path = json_object["path"].toString();
    if (json_object["memory_cells"].isArray()) {
      QJsonArray array = json_object["memory_cells"].toArray();
      for (auto key : array) memory_cells.append(key.toString());
    }
    hist_parts = json_object["hist_parts"].toInt();
    needs_user_input = json_object["needs_user_input"].toBool();
    fn_name = json_object["fn_name"].toString();
    stype = ScriptType::React;
  }
  /*! Script path. */
  QString path;
  /*! List of memory cells that will be passed to the script. */
  QStringList memory_cells;
  /*! The number of previous messages that will be passed to the script to understand the context. */
  int hist_parts = 0;
  /*! Name of function inside the script. */
  QString fn_name;
  /*! Indicates whether the script needs the entire text of the message entered by the user. */
  bool needs_user_input;
  
  /*! @brief Turns @a script into a JSON object. */
  QJsonObject to_json() const {
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

/*! @class DaemonizeableScriptMetadata
 *  @brief Contains metadata about a script.  */
class DaemonizeableScriptMetadata : public ScriptMetadata {
public:
  /*! Constructors. */
  DaemonizeableScriptMetadata() {}
  DaemonizeableScriptMetadata(const QJsonObject &json_object) {
    stype = ScriptType(json_object["stype"].toInt());
    cmd = json_object["cmd"].toString();
  }
  /*! Script's start command. */
  QString cmd;
  /*! Script type. */
  ScriptType stype = ScriptType::NoAction;
  
  /*! @brief Turns @a script into a JSON object. */
  virtual QJsonObject to_json() {
    return {
      {"cmd", cmd},
      {"stype", int(stype)}
    };
  }
};

/*! @brief Container for daemons of different types. */
typedef QList<DaemonizeableScriptMetadata *> DaemonsMeta;

/*! @class DaemonScript
 *  @brief Contains metadata about daemon script.  */
class DaemonScript : public DaemonizeableScriptMetadata {
public:
  /*! Constructors. */
  DaemonScript() : DaemonizeableScriptMetadata() {
    stype = ScriptType::Daemon;
  }
  DaemonScript(const QJsonObject &json_object) : DaemonizeableScriptMetadata(json_object) {
    stype = ScriptType::Daemon;
  }
  
  /*! @brief Turns @a script into a JSON object. */
  virtual QJsonObject to_json() {
    return {
      {"cmd", cmd},
      {"stype", int(stype)}
    };
  }
};

/*! @class ServerScript
 *  @brief Contains metadata about server script.  */
class ServerScript : public DaemonizeableScriptMetadata {
public:
  /*! Constructors. */
  ServerScript() : DaemonizeableScriptMetadata() {
    stype = ScriptType::Server;
  }
  ServerScript(const QJsonObject &json_object) : DaemonizeableScriptMetadata(json_object) {
    server_addr = QHostAddress(json_object["server_addr"].toString());
    if (server_addr.isNull()) server_addr = QHostAddress("127.0.0.1");
    server_port = quint16(json_object["server_port"].toInt());
    stype = ScriptType::Server;
  }
  /*! Server address. */
  QHostAddress server_addr;
  /*! Server port. */
  quint16 server_port;
  
  /*! @brief Turns @a script into a JSON object. */
  QJsonObject to_json() const {
    return {
      {"cmd", cmd},
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
    path = json_object["path"].toString();
    fn_name = json_object["fn_name"].toString();
    stype = ScriptType::CustomScan;
  }
  /*! Script path. */
  QString path;
  /*! Name of function inside the script. */
  QString fn_name;
  
  /*! @brief Turns @a script into a JSON object. */
  QJsonObject to_json() const {
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
  /*! Script path. */
  QString path;
  /*! Whether to send additional properties of selected phrases to the script. */
  bool send_adprops = false;
  /*! Name of function inside the script. */
  QString fn_name;
  
  /*! @brief Turns @a script into a JSON object. */
  QJsonObject to_json() const {
    return {
      {"path", path},
      {"stype", int(stype)},
      {"fn_name", fn_name},
      {"send_adprops", send_adprops}
    };
  }
};

/*! @class ScenarioScript
 *  @brief Contains metadata about scenario script.  */
class ScenarioScript : public DaemonizeableScriptMetadata {
public:
  /*! Constructors. */
  ScenarioScript() : DaemonizeableScriptMetadata() {
    stype = ScriptType::Scenario;
  }
  ScenarioScript(const QJsonObject &json_object) : DaemonizeableScriptMetadata(json_object) {
    server_addr = QHostAddress(json_object["server_addr"].toString());
    if (server_addr.isNull()) server_addr = QHostAddress("127.0.0.1");
    server_port = quint16(json_object["server_port"].toInt());
    stype = ScriptType::Scenario;
  }
  /*! Server address. */
  QHostAddress server_addr;
  /*! Server port. */
  quint16 server_port;
  
  /*! @brief Turns @a script into a JSON object. */
  QJsonObject to_json() const {
    return {
      {"cmd", cmd},
      {"stype", int(stype)},
      {"server_addr", server_addr.toString()},
      {"server_port", int(server_port)}
    };
  }
};

/*! @namespace ScriptsCast
 *  @brief Set of methods for casting script types.  */
namespace ScriptsCast {  
  // Functions described in `script.cpp`:
  ScriptMetadata *to_script(const QJsonObject &json_object);
  ScriptMetadata *to_script(QString expression);
  DaemonizeableScriptMetadata *to_daemon(const QJsonObject &json_object);
  DaemonizeableScriptMetadata *to_daemon(QString expression);
  QJsonObject to_json(ScriptMetadata *script);
  QString to_string(ScriptMetadata *script);
  QJsonObject to_json(DaemonizeableScriptMetadata *script);
  QString to_string(DaemonizeableScriptMetadata *script);
}

#endif
