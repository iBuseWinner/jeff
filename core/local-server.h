#ifndef LOCAL_SERVER_H
#define LOCAL_SERVER_H

#include "core/basis.h"
#include <QHostAddress>
#include <QJsonObject>
#include <QObject>
#include <QTcpServer>

/*!
 * @class Server
 * @brief Manages incoming connections and receives messages.
 */
class Server : public QObject {
  Q_OBJECT
public:
  // Functions described in `local-server.cpp`:
  Server(Basis *_basis, QObject *parent = nullptr);
  void start(QHostAddress address = QHostAddress::LocalHost, quint16 port = 8005);
  void handle_connection();
  
  /*! @brief Stops the server. */
  void stop() { server->close(); }
  
signals:
  /*! @brief Sends message to the screen on behalf of Jeff. */
  const QString &send(const QString &message);
  /*! @brief Sends a message to the screen and requests that it be handled as a user message. */
  const QString &send_to_respond(const QString &message);
  /*! @brief Reports an error in server listening. */
  QString server_error(QString error_text);
  
private:
  // Objects:
  Basis *basis = nullptr;
  QTcpServer *server = nullptr;
  
  // Functions described in `local-server.cpp`:
  bool authorize_connection(QString key);
};

#endif
