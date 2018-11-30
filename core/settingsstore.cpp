#include "settingsstore.h"
#include <QDebug>

SettingsStore::~SettingsStore() {
  // Prevents memory leaks.
  delete this->settings;
}

QVariant SettingsStore::read(QString key) {
  // Reads the setting by key.
  return this->settings->value(key);
}

QList<containerProperties> SettingsStore::read() {
  // Reads selection of AS from file.
  QString SettingsPath = this->settingsPath();
  QFile SelectionFile(SettingsPath + QDir::separator() +
                      this->selectionFileName);
  if (!SelectionFile.open(QIODevice::ReadOnly | QIODevice::Text))
    return QList<containerProperties>();
  QTextStream stream(&SelectionFile);
  QJsonParseError *err = new QJsonParseError;
  QJsonDocument DatabaseList =
      QJsonDocument::fromJson(stream.readAll().toUtf8(), err);
  if (err->error != QJsonParseError::NoError)
    return QList<containerProperties>();
  delete err;
  SelectionFile.close();
  QJsonArray Containers = DatabaseList.array();
  QList<containerProperties> Set;
  sqlite *SQ = new sqlite();
  for (int i = 0; i < Containers.size(); i++) {
    containerProperties Container =
        this->toContainerStruct(Containers.at(i).toObject());
    Container = SQ->optionsLoader(Container);
    Set.append(Container);
  }
  delete SQ;
  return Set;
}

void SettingsStore::write(QString key, QVariant data) {
  // Records the setting by key.
  this->settings->setValue(key, data);
}

void SettingsStore::write(QList<containerProperties> Set) {
  // Writes selection of AS into file.
  QString SettingsPath = this->settingsPath();
  QJsonArray Containers;
  sqlite *SQ = new sqlite();
  for (int i = 0; i < Set.length(); i++) {
    Containers.append(this->toJson(Set.at(i)));
    SQ->optionsWriter(Set.at(i));
  }
  delete SQ;
  QJsonDocument DatabaseList(Containers);
  QFile SelectionFile(SettingsPath + QDir::separator() +
                      this->selectionFileName);
  if (!SelectionFile.open(QIODevice::WriteOnly | QIODevice::Text))
    return;
  QTextStream stream(&SelectionFile);
  stream << DatabaseList.toJson(QJsonDocument::Indented);
  SelectionFile.close();
}

QString SettingsStore::settingsPath() {
  // Returns the setting path.
  QFileInfo *inf = new QFileInfo(this->settings->fileName());
  QString path = inf->absolutePath();
  delete inf;
  return path;
}

QJsonObject SettingsStore::toJson(containerProperties obj) {
  // Converts container structure into JSON object.
  QJsonObject converted{{"container", obj.container},
                        {"disabled", obj.disabled},
                        {"path", obj.path}};
  return converted;
}

containerProperties SettingsStore::toContainerStruct(QJsonObject obj) {
  // Converts JSON object into container structure.
  containerProperties converted;
  converted.container = obj.value("container").toString();
  converted.disabled = obj.value("disabled").toBool();
  converted.path = obj.value("path").toString();
  return converted;
}
