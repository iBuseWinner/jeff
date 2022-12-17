#include "notify-client.h"

/*! @brief The constructor. */
NotifyClient::NotifyClient(QObject *parent) : QObject(parent) {}

/*! @brief Notifies all scripts that have subscribed to notifications about a new message. */
void NotifyClient::notify(MessageData mdata) {
  if (not scenario)
    for (auto *script : scripts) send_event(mdata, script->server_addr, script->server_port);
  else send_event(mdata, scenario->server_addr, scenario->server_port);
}

/*! @brief Sends a message to the server over TCP. */
void NotifyClient::send_event(MessageData mdata, QHostAddress addr, quint16 port) {
  auto *socket = new QTcpSocket(this);
  connect(socket, &QTcpSocket::disconnected, socket, &QObject::deleteLater);
  connect(socket, &QTcpSocket::connected, this, [this, mdata, socket] {
    auto transport = mdata.to_json();
    QJsonDocument doc_to_script(transport);
    auto bytes_to_send = doc_to_script.toJson();
    socket->write(bytes_to_send);
    socket->disconnectFromHost();
  });
  socket->connectToHost(addr, port);
}

/*! @brief Subscribes the script to notifications. */
void NotifyClient::subscribe(ServerScript *script) { scripts.append(script); }
/*! @brief Unsubscribes the script from notifications. */
void NotifyClient::unsubscribe(ServerScript *script) { scripts.removeOne(script); }
/*! @brief Unsubscribes all scripts from notifications. */
void NotifyClient::unsubscribe_all() { scripts.clear(); }
/*! @brief Organizes the temporary transfer of all new messages only to the scenario script. */
void NotifyClient::set_scenario_listener(ScenarioScript *script) { scenario = script; }
/*! @brief Disables scenario notification. */
void NotifyClient::unset_scenario_listener() { scenario = nullptr; }
