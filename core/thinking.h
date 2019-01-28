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
  Q_PROPERTY(QString resultOfThinking MEMBER stringResult NOTIFY
                 resultOfThinkingChanged)
public:
  thinking();
  void putExpression(QString userExpression);
  void startProcessing();
  QList<containerProperties> getSelection();
  bool preparePlugins(QString userExpression);
  linksMicroMap handlePlugins(QList<containerRow> containerMaterial,
                              bool hasAdditionalProperties);
  globalExpressionNetworkMap
  microMapCombinator(QList<linksMicroMap> selectionSearchResults);
  QStringList sorting(QStringList keys);
  void selectReagents(globalExpressionNetworkMap genm);
  QString get();

signals:
  QString resultOfThinkingChanged();

private:
  QString simplifier(QString expression);
  sqlite *SQ{};
  Q_DISABLE_COPY(thinking)
  QString currentInput;
  QString stringResult = "";
};

#endif // THINKING_H
