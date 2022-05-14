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
  int number_of_hist_messages;
  
  /*! @brief Turns @a script into a JSON object. */
  QJsonObject to_json() {
    QJsonArray memory_cells_array;
    for (auto str : memory_cells) memory_cells_array.append(str);
    return {
      {"path", path},
      {"action", int(action)},
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
      {"action", int(action)},
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
      {"action", int(action)},
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
  CustomComposeScript(const QJsonObject &json_object) : ScriptMetadata(json_object) {}
}

#endif
