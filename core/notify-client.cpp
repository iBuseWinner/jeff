#include "notify-client.h"

/*! @brief The constructor. */
NotifyClient::NotifyClient(Basis *_basis, QObject *parent = nullptr) : QObject(parent), basis(_basis) {}

/*! @brief TBD */
void NotifyClient::notify(MessageData mdata, QList<ScriptMetadata> scripts) {
  for (auto &script : scripts) if (script.notify) send_event(mdata, script.server_addr, script.server_port);
}

/*! @brief TBD */
void NotifyClient::send_event(MessageData mdata, QHostAddress addr, quint16 port) {
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
