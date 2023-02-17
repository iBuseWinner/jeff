#ifndef NOTIFY_CLIENT
#define NOTIFY_CLIENT

#include "core-kit/basis.hpp"
#include "core-kit/extensions/extension.hpp"
#include "core-kit/model/message.hpp"
#include "yelloger.h"
#include <QHostAddress>
#include <QJsonObject>
#include <QList>
#include <QTcpSocket>

/*! @class NotifyClient
 *  @brief Sends new messages over the network.  */
class NotifyClient : public QObject {
  Q_OBJECT
public:
  // Functions described in `notify-client.cpp`:
  NotifyClient(QObject *parent = nullptr);
  void subscribe(ExtensionMeta *extension_meta);
  void unsubscribe(ExtensionMeta *extension_meta);
  void unsubscribe_all();
  void notify(MessageMeta *msg_meta, bool no_jck_output = false);
  void notify_scenario_first_time(QString auth_key);
  void notify_only(ExtensionMeta *target, MessageMeta *msg_meta);
  void set_scenario(ScenarioServerMeta _scenario_server_meta);
  void finish_scenario();
  void unset_scenario();
  
private:
  // Objects:
  ExtensionsMeta extensions_meta;
  bool is_scenario_running = false;
  ScenarioServerMeta scenario_server_meta;
  
  // Functions described in `notify-client.cpp`:
  void send_event(MessageMeta *msg_meta, QHostAddress addr, quint16 port);
};

#endif
