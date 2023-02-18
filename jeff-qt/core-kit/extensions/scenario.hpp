#ifndef SCENARIO_H
#define SCENARIO_H

#include <QHostAddress>
#include <QList>
#include <QString>

/*! @brief The address and port of the server responsible for the specific scenario. */
struct ScenarioServerMeta {
  QHostAddress server_addr;
  quint16 server_port;
  QString auth_key;
  QString name;
};

/*! @brief List of scenarios. */
typedef QList<ScenarioServerMeta> Scenarios;

#endif
