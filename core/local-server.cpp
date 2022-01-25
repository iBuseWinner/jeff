#include "local-server.h"

/*!
 * @fn Server::Server
 * @brief The constructor.
 */
Server::Server(Basis *_basis, QObject *parent) : QObject(parent), basis(_basis) {
  server = new QTcpServer(this);
  connect(server, &QTcpServer::newConnection, this, &Server::handle_connection);
}

/*!
 * @fn Server::start
 * @brief Starts the server at given @a address and @a port.
 */
void Server::start(QHostAddress address, quint16 port) {
  if (not server->listen(address, port)) {
    emit server_error(
      tr("Unable to start server. Perhaps the port %1 is busy.").arg(QString::number(port))
    );
  }
}

/*!
 * 
 */
void Server::handle_connection() {
  auto *connection = server->nextPendingConnection();
  connect(connection, &QAbstractSocket::disconnected, connection, &QObject::deleteLater);
  auto byte_array = connection->readAll();
  if (byte_array.isEmpty()) { connection->disconnectFromHost(); return; }
  auto doc = QJsonDocument::fromJson(byte_array);
  if (doc.isEmpty()) { connection->disconnectFromHost(); return; }
  auto data = doc.object();
  if (not authorize_connection(data[basis->auth_key])) { connection->disconnectFromHost(); return; }
  
}

/*!
 * @fn Server::authorize_connection
 * @brief If protection against malicious connections is enabled, authorizes the connection by key.
 */
bool Server::authorize_connection(const QString &key) {
  if (not (*basis)[basis->serverAuthorizationOnSt].toBool()) return true;
  if (key == (*basis)[basis->serverAuthKeySt]) return true;
  return false;
}
