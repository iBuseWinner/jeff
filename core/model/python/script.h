#ifndef SCRIPT_H
#define SCRIPT_H

#include <QHostAddress>
#include <QMap>

/*! @enum ScriptActions
 *  @brief TBD  */
enum ScriptActions { NoAction, React, Startup, CustomScan, CustomCompose };

/*! @enum ToNotify
 *  @brief TBD  */
enum ToNotify { NoNotify, NotifyAtTCP };

/*! @enum ToDaemonize
 *  @brief TBD  */
enum ToDaemonize { NotDaemon, Daemonize };

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
    notify = ToNotify(json_object["notify"].toBool());
    daemonize = ToDaemonize(json_object["daemonize"].toBool());
    server_addr = QHostAddress(json_object["server_addr"].toString());
    server_port = quint16(json_object["server_port"].toInt());
  }
  /*! The path to the script on disk. */
  QString path;
  /*! TBD */
  ScriptActions action;
  /*! TBD */
  QString fn_name;
  /*! TBD */
  ToDaemonize daemonize;
  /*! TBD */
  ToNotify notify;
  /*! TBD */
  QHostAddress server_addr;
  /*! TBD */
  quint16 server_port;
  
  /*! @brief Turns @a script into a JSON object. */
  QJsonObject to_json() {
    return {
      {"path", path},
      {"action", int(action)},
      {"fn_name", fn_name},
      {"notify", bool(notify)},
      {"daemonize", bool(daemonize)},
      {"server_addr", server_addr.toString()},
      {"server_port", int(server_port)}
    };
  }
};

/*! @typedef Scripts
 *  @brief Contains metadata about multiple scripts.  */
typedef QList<ScriptMetadata> Scripts;

#endif
