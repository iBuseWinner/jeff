#ifndef THINKING_H
#define THINKING_H

#include "core/containersstruct.h"
#include "core/handlers.h"
#include "core/settingsstore.h"
#include "core/sqlite.h"
#include <QMap>
#include <QObject>
#include <QPair>
#include <QRandomGenerator>
#include <QStringList>
#include <QTime>
#include <utility>

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
  thinking();
  QString get(QString userExpression);
  QList<containerProperties> getSelection();
  QString preparePlugins(QString userExpression);
  linksMicroMap handlePlugins(QList<containerRow> containerMaterial,
                              bool hasAdditionalProperties);
  globalExpressionNetworkMap
  microMapCombinator(sqlite *SQ, QList<linksMicroMap> selectionSearchResults);
  QStringList sorting(QString userExpression, QStringList keys);
  QString selectReagents(QString userExpression, globalExpressionNetworkMap genm);

private:
  QString simplifier(QString expression);
  Q_DISABLE_COPY(thinking)
};

#endif // THINKING_H
