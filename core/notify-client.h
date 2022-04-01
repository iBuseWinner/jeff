#ifndef NOTIFY_CLIENT
#define NOTIFY_CLIENT

#include "core/basis.h"
#include "core/model/message.h"
#include "core/model/python/script.h"
#include <QHostAddress>
#include <QJsonObject>
#include <QList>
#include <QTcpSocket>

/*! @class NotifyClient
 *  @brief TBD  */
class NotifyClient : public QObject {
  Q_OBJECT
public:
  // Functions described in `notify-client.cpp`:
  NotifyClient(Basis *_basis, QObject *parent = nullptr);
  
private:
  // Objects:
  Basis *basis = nullptr;
  
  // Functions described in `notify-client.cpp`:
  void send_event(MessageData)
};

#endif
