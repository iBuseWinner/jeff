#include "local-server.h"

/*! @brief The constructor. */
Server::Server(Basis *_basis, QObject *parent) : QObject(parent), basis(_basis) {
  server = new QTcpServer(this);
  connect(server, &QTcpServer::newConnection, this, &Server::handle_connection);
}

/*! @brief Starts the server at given @a address and @a port. */
void Server::start(QHostAddress address, quint16 port) {
  if (not server->listen(address, port)) {
    emit server_error(
      tr("Unable to start server. Perhaps the port %1 is busy.").arg(QString::number(port))
    );
  }
}

/*! @brief Handles incoming connections processing connection data. */
void Server::handle_connection() {
  auto *connection = server->nextPendingConnection();
  connect(connection, &QAbstractSocket::disconnected, connection, &QObject::deleteLater);
  connect(connection, &QTcpSocket::readyRead, this, [this, connection] {
    QByteArray byte_array;
    while (connection->bytesAvailable()) byte_array.append(connection->readAll());
    if (byte_array.isEmpty()) { connection->disconnectFromHost(); return; }
    auto doc_from_script = QJsonDocument::fromJson(byte_array);
    if (doc_from_script.isEmpty()) { connection->disconnectFromHost(); return; }
    auto data = doc_from_script.object();
    if (not authorize_connection(data[basis->authKeyWk].toString())) {
      connection->disconnectFromHost();
      return;
    }
    basis->handle_from_script(data);
    auto transport = basis->handle_to_script(data);
    if (not transport.isEmpty()) {
      QJsonDocument doc_to_script(transport);
      auto bytes_to_send = doc_to_script.toJson();
      connection->write(bytes_to_send);
    }
    connection->disconnectFromHost();
  });
}

/*! @brief If protection against malicious connections is enabled, authorizes the connection by key. */
bool Server::authorize_connection(const QString &key) {
  if (not (*basis)[basis->serverAuthorizationOnSt].toBool()) return true;
  if (key == (*basis)[basis->serverAuthKeySt]) return true;
  return false;
}
