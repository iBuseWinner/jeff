#ifndef SCRIPT_H
#define SCRIPT_H

#include <QHostAddress>
#include <QMap>

/*! @enum ScriptType
 *  @brief Specifies the types of scripts that are used in Jeff.
 *  @details There are the following types of scripts:
 *  1. React - Reacts to user expressions, added directly to databases
 *  2. Startup - Run once when Jeff starts
 *  3. Daemon - Run as daemons and interact with Jeff's server as needed
 *  4. Server - Run as daemons, receive notifications of new messages and interact with the server
 *  5. CustomScan - Scans and responds to user input, replacing Jeff's NLPmodule
 *  6. CustomCompose - Receives response options for user input from Jeff and composes the response  */
enum ScriptType { NoAction, React, Startup, Daemon, Server, CustomScan, CustomCompose };

/*! @class ScriptMetadata
 *  @brief Contains metadata about a script.  */
class ScriptMetadata {
public:
  /*! Constructors. */
  ScriptMetadata() {}
  ScriptMetadata(const QJsonObject &json_object) {
    path = json_object["path"].toString();
    stype = ScriptType(json_object["stype"].toInt());
    fn_name = json_object["fn_name"].toString();
  }
  /*! Script path. */
  QString path;
  /*! Script type. */
  ScriptType stype = ScriptType::NoAction;
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

/*! @brief Container for scripts of different types. */
typedef QList<ScriptMetadata *> Scripts;

/*! @class ReactScript
 *  @brief Contains metadata about reagent script.  */
class ReactScript : public ScriptMetadata {
public:
  /*! Constructors. */
  ReactScript() : ScriptMetadata() {}
  ReactScript(const QJsonObject &json_object) : ScriptMetadata(json_object) {
    if (json_object["memory_cells"].isArray()) {
      auto array = json_object["memory_cells"].toArray();
      for (auto key : array) memory_cells.append(key.toString());
    }
    number_of_hist_messages = json_object["number_of_hist_messages"].toInt();
  }
  QStringList memory_cells;
  int number_of_hist_messages = 0;
  
  /*! @brief Turns @a script into a JSON object. */
  QJsonObject to_json() {
    QJsonArray memory_cells_array;
    for (auto str : memory_cells) memory_cells_array.append(str);
    return {
      {"path", path},
      {"stype", int(stype)},
      {"fn_name", fn_name},
      {"memory_cells", memory_cells_array},
      {"number_of_hist_messages", number_of_hist_messages}
    };
  }
}

/*! @class StartupScript
 *  @brief Contains metadata about startup script.  */
class StartupScript : public ScriptMetadata {
public:
  /*! Constructors. */
  StartupScript() : ScriptMetadata() {}
  StartupScript(const QJsonObject &json_object) : ScriptMetadata(json_object) {
    if (json_object["memory_cells"].isArray()) {
      auto array = json_object["memory_cells"].toArray();
      for (auto key : array) memory_cells.append(key.toString());
    }
  }
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
}

/*! @class DaemonScript
 *  @brief Contains metadata about daemon script.  */
class DaemonScript : public ScriptMetadata {
public:
  /*! Constructors. */
  DaemonScript() : ScriptMetadata() {}
  DaemonScript(const QJsonObject &json_object) : ScriptMetadata(json_object) {}
}

/*! @class ServerScript
 *  @brief Contains metadata about server script.  */
class ServerScript : public ScriptMetadata {
public:
  /*! Constructors. */
  ServerScript() : ScriptMetadata() {}
  ServerScript(const QJsonObject &json_object) : ScriptMetadata(json_object) {
    server_addr = QHostAddress(json_object["server_addr"].toString());
    server_port = quint16(json_object["server_port"].toInt());
  }
  /*! Server address. */
  QHostAddress server_addr;
  /*! Server port. */
  quint16 server_port;
  
  /*! @brief Turns @a script into a JSON object. */
  QJsonObject to_json() {
    QJsonArray memory_cells_array;
    for (auto str : memory_cells) memory_cells_array.append(str);
    return {
      {"path", path},
      {"stype", int(stype)},
      {"fn_name", fn_name},
      {"server_addr", server_addr.toString()},
      {"server_port", int(server_port)}
    };
  }
}

/*! @class CustomScanScript
 *  @brief Contains metadata about custom scanner.  */
class CustomScanScript : public ScriptMetadata {
public:
  /*! Constructors. */
  CustomScanScript() : ScriptMetadata() {}
  CustomScanScript(const QJsonObject &json_object) : ScriptMetadata(json_object) {}
}

/*! @class CustomComposeScript
 *  @brief Contains metadata about custom composer.  */
class CustomComposeScript : public ScriptMetadata {
public:
  /*! Constructors. */
  CustomComposeScript() : ScriptMetadata() {}
  CustomComposeScript(const QJsonObject &json_object) : ScriptMetadata(json_object) {
    send_adprops = json_object["send_adprops"].toBool();
  }
  /*! Whether to send additional properties of selected phrases to the script. */
  bool send_adprops = false;
  
  /*! @brief Turns @a script into a JSON object. */
  QJsonObject to_json() {
    QJsonArray memory_cells_array;
    for (auto str : memory_cells) memory_cells_array.append(str);
    return {
      {"path", path},
      {"stype", int(stype)},
      {"fn_name", fn_name},
      {"send_adprops", send_adprops}
    };
  }
}

/*! @namespace ScriptsCast
 *  @brief Set of methods for casting script types.  */
namespace ScriptsCast {  
  /*! @brief Translates @a json_object into a script. */
  ScriptMetadata *to_script(const QJsonObject &json_object) {
    ScriptType stype = ScriptType(json_object["stype"].toInt());
    if (stype == ScriptType::React) {
      auto *script = new ReactScript(json_object);
      return script;
    } else if (stype == ScriptType::Startup) {
      auto *script = new StartupScript(json_object);
      return script;
    } else if (stype == ScriptType::Daemon) {
      auto *script = new DaemonScript(json_object);
      return script;
    } else if (stype == ScriptType::Server) {
      auto *script = new ServerScript(json_object);
      return script;
    } else if (stype == ScriptType::CustomScan) {
      auto *script = new CustomScanScript(json_object);
      return script;
    } else if (stype == ScriptType::CustomCompose) {
      auto *script = new CustomComposeScript(json_object);
      return script;
    } else return nullptr;
  }
  
  /*! @brief Translates @a expression into a script. */
  ScriptMetadata *to_script(QString expression) {
    QJsonParseError errors;
    QJsonDocument document = QJsonDocument::fromJson(expression.toUtf8(), &errors);
    if (errors.error != QJsonParseError::NoError) return nullptr;
    auto json_object = document.object();
    return to_script(json_object);
  }
}

#endif
