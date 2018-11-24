#include "thinking.h"
#include <QDebug>

thinking::thinking() {}

void thinking::putExpression(QString userExpression) {
  this->currentInput = userExpression;
}

void thinking::startProcessing() {
  // Forms associative system core and finds reagents.
  // Prepares user expression...
  if (this->preparePlugins(currentInput))
    return;
  // For each container in AS selection...
  QList<containerProperties> ASSelection = this->getSelection();
  QList<linksMicroMap> containerMaterials;
  SQ = new sqlite();
  foreach (containerProperties table, ASSelection) {
    // Gets the activators found in the custom expression and the reagent links
    QList<QPair<QString, QString>> activatorsAndLinks =
        SQ->scan(table.path, table.container, currentInput);
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
      this->microMapCombinator(containerMaterials);
  delete SQ;
  this->selectReagents(genm);
}

QList<containerProperties> thinking::getSelection() {
  // Gets AS selection.
  SettingsStore *ST = new SettingsStore();
  QList<containerProperties> selection = ST->read();
  delete ST;
  return selection;
}

bool thinking::preparePlugins(QString userExpression) {
  // Handles expression by Python scripts and built-in methods.
  // Here will be the expression processing code in scripts...
  handlers HD;
  currentInput = HD.purifyString(userExpression);
  // < ... >
  return false;
}

linksMicroMap thinking::handlePlugins(QList<containerRow> containerMaterial,
                                      bool hasAdditionalProperties) {
  // Handles all answer expressions by Python scripts and built-in methods.
  linksMicroMap compressedContainerMaterial;
  QMap<QString, QList<int>> compressedLines;
  foreach (containerRow expandedLine, containerMaterial) {
    // If there isn't current activator in compressed lines...
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
thinking::microMapCombinator(QList<linksMicroMap> selectionSearchResults) {
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

void thinking::selectReagents(globalExpressionNetworkMap genm) {
  QStringList keys = genm.allFoundReagents.keys();
  QString tempResult;
  foreach (QString key, keys) {
    currentInput.remove(key);
    QRandomGenerator rand(quint32(QTime::currentTime().msec()));
    tempResult += genm.allFoundReagents.value(key).at(
        rand.bounded(genm.allFoundReagents.value(key).length())) + " ";
    stringResult = tempResult;
  }
}

QString thinking::get() {
  // Returns result.
  return this->stringResult;
}
