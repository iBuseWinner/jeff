#include "notify-client.hpp"

/*! @brief The constructor. */
NotifyClient::NotifyClient(QObject *parent) : QObject(parent) {}

/*! @brief Notifies all extensions that have subscribed to notifications about a new message. */
void NotifyClient::notify(MessageMeta msg_meta, bool no_jck_output) {
  Yellog::Trace("Notifier working...");
  if (not is_scenario_running)
    for (auto *ext_m : extensions_meta) {
      if ((ext_m->always_send) or (not ext_m->always_send and no_jck_output))
        send_event(msg_meta, ext_m->server_addr, ext_m->server_port);
    }
  else send_event(msg_meta, scenario_server_meta.server_addr, scenario_server_meta.server_port);
}

/*! @brief Passes authentication data to the extension. */
void NotifyClient::notify_scenario_first_time(MessageMeta msg_meta, QString auth_key) {
  if (not is_scenario_running) return;
  auto *socket = new QTcpSocket(this);
  connect(socket, &QTcpSocket::disconnected, socket, &QObject::deleteLater);
  connect(socket, &QTcpSocket::connected, this, [this, msg_meta, socket, auth_key] {
    auto transport = msg_meta.to_json();
    transport[Basis::scenarioTokenWk] = auth_key;
    QJsonDocument doc_to_script(transport);
    auto bytes_to_send = doc_to_script.toJson();
    socket->write(bytes_to_send);
    socket->disconnectFromHost();
  });
  socket->connectToHost(scenario_server_meta.server_addr, scenario_server_meta.server_port);
}

/*! @brief Notifies the extension that the script has finished. */
void NotifyClient::finish_scenario() {
  if (not is_scenario_running) return;
  auto *socket = new QTcpSocket(this);
  connect(socket, &QTcpSocket::disconnected, socket, &QObject::deleteLater);
  connect(socket, &QTcpSocket::connected, this, [this, socket] {
    QJsonObject transport;
    transport[Basis::scenarioFinishWk] = true;
    QJsonDocument doc_to_script(transport);
    auto bytes_to_send = doc_to_script.toJson();
    socket->write(bytes_to_send);
    socket->disconnectFromHost();
  });
  socket->connectToHost(scenario_server_meta.server_addr, scenario_server_meta.server_port);
  unset_scenario();
}

/*! @brief Sends a message to the server over TCP. */
void NotifyClient::send_event(MessageMeta msg_meta, QHostAddress addr, quint16 port) {
  auto *socket = new QTcpSocket(this);
  connect(socket, &QTcpSocket::disconnected, socket, &QObject::deleteLater);
  connect(socket, &QTcpSocket::connected, this, [this, msg_meta, socket] {
    auto transport = msg_meta.to_json();
    QJsonDocument doc_to_script(transport);
    auto bytes_to_send = doc_to_script.toJson();
    socket->write(bytes_to_send);
    socket->disconnectFromHost();
  });
  socket->connectToHost(addr, port);
}

/*! @brief Subscribes the extension to notifications. */
void NotifyClient::subscribe(ExtensionMeta *extension_meta) { extensions_meta.append(extension_meta); }
/*! @brief Unsubscribes the extension from notifications. */
void NotifyClient::unsubscribe(ExtensionMeta *extension_meta) { extensions_meta.removeOne(extension_meta); }
/*! @brief Unsubscribes all extensions from notifications. */
void NotifyClient::unsubscribe_all() { extensions_meta.clear(); }
/*! @brief Organizes the temporary transfer of all new messages only to the scenario. */
void NotifyClient::set_scenario(ScenarioServerMeta _scenario_server_meta) {
  scenario_server_meta = _scenario_server_meta;
  is_scenario_running = true;
}
/*! @brief Disables scenario notification. */
void NotifyClient::unset_scenario() { is_scenario_running = false; }
