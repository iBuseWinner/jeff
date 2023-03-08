#include "notify-client.hpp"

/*! @brief The constructor. */
NotifyClient::NotifyClient(QObject *parent) : QObject(parent) {}

/*! @brief Notifies all extensions that have subscribed to notifications about a new message. */
void NotifyClient::notify(MessageMeta *msg_meta, bool no_jck_output) {
  Yellog::Trace("Notifier working...");
  if (not is_scenario_running) {
    for (auto *ext_m : extensions_meta) {
      if ((ext_m->always_send and not no_jck_output) or (not ext_m->always_send and no_jck_output)) {
        send_event(msg_meta, ext_m->server_addr, ext_m->server_port);
      }
    }
  } else if (msg_meta->author == Author::User) {
    send_event(msg_meta, scenario_meta.server_addr, scenario_meta.server_port);
  } else {
    for (auto *ext_m : extensions_meta) {
      if (
        ext_m->always_send and
        (scenario_meta.server_addr != ext_m->server_addr or scenario_meta.server_port != ext_m->server_port)
      ) {
        send_event(msg_meta, ext_m->server_addr, ext_m->server_port);
      }
    }
  }
}

/*! @brief Passes authentication data to the extension. */
void NotifyClient::notify_scenario_first_time(QString auth_key) {
  if (not is_scenario_running) return;
  auto *socket = new QTcpSocket(this);
  connect(socket, &QTcpSocket::disconnected, socket, &QObject::deleteLater);
  connect(socket, &QTcpSocket::connected, this, [this, socket, auth_key] {
    auto transport = QJsonObject();
    transport[Basis::scenarioTokenWk] = auth_key;
    QJsonDocument doc_to_script(transport);
    auto bytes_to_send = doc_to_script.toJson();
    socket->write(bytes_to_send);
    socket->disconnectFromHost();
  });
  socket->connectToHost(scenario_meta.server_addr, scenario_meta.server_port);
}

/*! @brief TBD */
void NotifyClient::notify_about_queued(ScenarioServerMeta _scenario_meta) {
  auto *socket = new QTcpSocket(this);
  connect(socket, &QTcpSocket::disconnected, socket, &QObject::deleteLater);
  connect(socket, &QTcpSocket::connected, this, [this, socket] {
    auto transport = QJsonObject();
    transport[Basis::scenarioQueuedWk] = true;
    QJsonDocument doc_to_script(transport);
    auto bytes_to_send = doc_to_script.toJson();
    socket->write(bytes_to_send);
    socket->disconnectFromHost();
  });
  socket->connectToHost(_scenario_meta.server_addr, _scenario_meta.server_port);
}

/*! @brief TBD */
void NotifyClient::notify_only(ExtensionMeta *target, MessageMeta *msg_meta) {
  for (auto *extension_meta : extensions_meta)
    if (target == extension_meta)
      send_event(msg_meta, target->server_addr, target->server_port);
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
  socket->connectToHost(scenario_meta.server_addr, scenario_meta.server_port);
  unset_scenario();
}

/*! @brief Sends a message to the server over TCP. */
void NotifyClient::send_event(MessageMeta *msg_meta, QHostAddress addr, quint16 port) {
  auto *socket = new QTcpSocket(this);
  connect(socket, &QTcpSocket::disconnected, socket, &QObject::deleteLater);
  connect(socket, &QTcpSocket::connected, this, [this, msg_meta, socket] {
    auto transport = msg_meta->to_json();
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
void NotifyClient::set_scenario(ScenarioServerMeta _scenario_meta) {
  scenario_meta = _scenario_meta;
  is_scenario_running = true;
}
/*! @brief Disables scenario notification. */
void NotifyClient::unset_scenario() { is_scenario_running = false; }
