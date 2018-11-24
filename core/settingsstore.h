#ifndef SETTINGSSTORE_H
#define SETTINGSSTORE_H

#include "core/containersstruct.h"
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QIODevice>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QList>
#include <QSettings>
#include <QString>
#include <QTextStream>
#include <QVariant>

class SettingsStore {

public:
  ~SettingsStore();
  QVariant read(QString key);
  QList<containerProperties> read();
  void write(QString key, QVariant data);
  void write(QList<containerProperties> Set);
  QString settingsPath();
  const QString company = "ACLO";
  const QString application = "ASW";
  QJsonObject toJson(containerProperties obj);
  containerProperties toContainerStruct(QJsonObject obj);

private:
  const QString selectionFileName = "selection.json";
  QSettings *settings = new QSettings(
      QSettings::IniFormat, QSettings::UserScope, company, application);
};

#endif // SETTINGSSTORE_H
