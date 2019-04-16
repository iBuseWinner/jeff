#ifndef THINKING_H
#define THINKING_H

#include "core/containersstruct.h"
#include "core/handlers.h"
#include "core/settingsstore.h"
#include "core/sqlite.h"
#include "core/standardtemplates.h"
#include "widgets/a_message.h"
#include <QMap>
#include <QObject>
#include <QPair>
#include <QRandomGenerator>
#include <QStringList>
#include <QTime>

struct containerRow {
  QString activator;
  int reagentAddress;
  QMap<QString, QString> reagentProperties;
};

struct linksMicroMap {
  QMap<QString, QList<int>> expandedLines;
  containerProperties parent;
};

struct globalExpressionNetworkMap {
  QMap<QString, QStringList> allFoundReagents;
};

class thinking : public QObject {
  Q_OBJECT
public:
  AMessage *get(const QString& userExpression, AMessage::A Author,
                QWidget *parent = nullptr);
  QString think(const QString& userExpression /*, AkiwakeMessage *msg*/);
  QString preparePlugins(QString userExpression, AMessage *msg);
  linksMicroMap handlePlugins(QList<containerRow> containerMaterial,
                              bool hasAdditionalProperties/*,
                              AkiwakeMessage *msg*/);
  globalExpressionNetworkMap
  microMapCombinator(sqlite *SQ, QList<linksMicroMap> selectionSearchResults);
  QStringList sorting(const QString& userExpression, QStringList keys);
  QString selectReagents(QString userExpression,
                         globalExpressionNetworkMap genm);

private:
  Q_DISABLE_COPY(thinking)
  QString simplifier(QString expression);
};

#endif // THINKING_H
