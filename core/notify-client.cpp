#include "notify-client.h"

/*! @brief The constructor. */
NotifyClient::NotifyClient(QObject *parent) : QObject(parent) {}

/*! @brief TBD */
void NotifyClient::notify(MessageData mdata) {
  for (auto *script : scripts) send_event(mdata, script->server_addr, script->server_port);
}

/*! @brief TBD */
void NotifyClient::subscribe(ServerScript *script) { scripts.append(script); }

/*! @brief TBD */
void NotifyClient::unsubscribe(ServerScript *script) { scripts.remove(script); }

/*! @brief TBD */
void NotifyClient::unsubscribe_all() { scripts.clear(); }

/*! @brief TBD */
void NotifyClient::send_event(const MessageData &mdata, QHostAddress addr, quint16 port) {
  auto socket = new QTcpSocket(this);
  socket->connectToHost(addr, port);
  connect(socket, &QTcpSocket::disconnected, socket, &QObject::deleteLater);
  connect(socket, &QTcpSocket::connected, this, [this, mdata] {
    auto transport = mdata.to_json();
    QJsonDocument doc_to_script(transport);
    auto bytes_to_send = doc_to_script.toJson();
    socket->write(bytes_to_send);
    socket->disconnectFromHost();
  });
}
