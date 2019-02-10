#include "thinking.h"

QString thinking::get(QString userExpression) {
  // Forms associative system core and finds reagents.
  // Prepares user expression...
  userExpression = this->preparePlugins(userExpression);
  if (userExpression == "")
    return "";
  QString stringResult = "";
  // For each container in AS selection...
  QList<containerProperties> ASSelection = this->getSelection();
  QList<linksMicroMap> containerMaterials;
  auto *SQ = new sqlite();
  foreach (containerProperties table, ASSelection) {
    // Gets the activators found in the custom expression and the reagent links
    QList<QPair<QString, QString>> activatorsAndLinks =
        SQ->scan(table.path, table.container, userExpression);
    QList<containerRow> containerMaterial;
    bool hasAdditionalProperties =
        SQ->hasAdditionalProperties(table.path, table.container);
    // For each mind connection...
    QPair<QString, QString> mindConnection;
    foreach (mindConnection, activatorsAndLinks) {
      QStringList links = mindConnection.second.split(",");
      // ...we get a lot of single connections...
      foreach (QString link, links) {
        containerRow expandedLine;
        expandedLine.activator = mindConnection.first;
        expandedLine.reagentAddress = link.toInt();
        if (hasAdditionalProperties)
          expandedLine.reagentProperties = SQ->scanAP(
              table.path, table.container, expandedLine.reagentAddress);
        containerMaterial.append(expandedLine);
      }
    }
    // We have the container row's list now...
    linksMicroMap compressedContainerMaterial =
        this->handlePlugins(containerMaterial, hasAdditionalProperties);
    compressedContainerMaterial.parent = table;
    containerMaterials.append(compressedContainerMaterial);
  }
  // And we have the all needed materials. Let's compose it...
  globalExpressionNetworkMap genm =
      this->microMapCombinator(SQ, containerMaterials);
  delete SQ;
  return this->selectReagents(userExpression, genm);
}

QList<containerProperties> thinking::getSelection() {
  // Gets AS selection.
  auto *ST = new SettingsStore();
  QList<containerProperties> selection = ST->read();
  delete ST;
  return selection;
}

QString thinking::preparePlugins(QString userExpression) {
  // Handles expression by Python scripts and built-in methods.
  // Here will be the expression processing code in scripts...
  // < ... >
  return this->simplifier(userExpression);
}

QString thinking::simplifier(QString expression) {
  handlers HD;
  return HD.purifyString(std::move(expression));
}

linksMicroMap thinking::handlePlugins(QList<containerRow> containerMaterial,
                                      bool hasAdditionalProperties) {
  // Handles all answer expressions by Python scripts and built-in methods.
  linksMicroMap compressedContainerMaterial;
  QMap<QString, QList<int>> compressedLines;
  foreach (containerRow expandedLine, containerMaterial) {
    // If there isn't current activator in compressed lines...
    expandedLine.activator = this->simplifier(expandedLine.activator);
    if (!compressedLines.contains(expandedLine.activator)) {
      QList<int> addresses;
      compressedLines.insert(expandedLine.activator, addresses);
    }
    // If there isn't current address in list of addresses of the current
    // activator in compressed lines...
    if (!compressedLines.value(expandedLine.activator)
             .contains(expandedLine.reagentAddress)) {
      QList<int> addresses = compressedLines.value(expandedLine.activator);
      addresses.append(expandedLine.reagentAddress);
      compressedLines.insert(expandedLine.activator, addresses);
    }
  }
  if (hasAdditionalProperties) {
    // < ... >
  }
  compressedContainerMaterial.expandedLines = compressedLines;
  return compressedContainerMaterial;
}

globalExpressionNetworkMap
thinking::microMapCombinator(sqlite *SQ, QList<linksMicroMap> selectionSearchResults) {
  globalExpressionNetworkMap genm;
  foreach (linksMicroMap microMap, selectionSearchResults) {
    foreach (QList<int> links, microMap.expandedLines) {
      QString activator = microMap.expandedLines.key(links);
      QStringList reagents;
      if (genm.allFoundReagents.contains(activator))
        reagents = genm.allFoundReagents.value(activator);
      foreach (int link, links) {
        QString reagent =
            SQ->scan(microMap.parent.path, microMap.parent.container, link)
                .first;
        if ((!reagents.contains(reagent)) && (reagent != ""))
          reagents.append(reagent);
      }
      genm.allFoundReagents.insert(activator, reagents);
    }
  }
  return genm;
}

QStringList thinking::sorting(QString userExpression, QStringList keys) {
  if (keys.length() <= 1)
    return keys;
  QString partition = keys.takeAt(int(keys.length() / 2));
  QStringList early, late;
  foreach (QString key, keys) {
    if (userExpression.indexOf(key) > userExpression.indexOf(partition))
      late.append(key);
    else
      early.append(key);
  }
  QStringList sorted;
  early = this->sorting(userExpression, early);
  foreach (QString key, early) { sorted.append(key); }
  sorted.append(partition);
  late = this->sorting(userExpression, late);
  foreach (QString key, late) { sorted.append(key); }
  return sorted;
}

QString thinking::selectReagents(QString userExpression, globalExpressionNetworkMap genm) {
  if (genm.allFoundReagents.keys().length() == 0)
    return "";
  QStringList keys = this->sorting(userExpression, genm.allFoundReagents.keys());
  QString tempResult;
  foreach (QString key, keys) {
    userExpression.remove(key);
    QRandomGenerator rand(quint32(QTime::currentTime().msec()));
    tempResult += genm.allFoundReagents.value(key).at(
                      rand.bounded(genm.allFoundReagents.value(key).length())) +
                  " ";
  }
  return tempResult;
}
