#ifndef SETTINGSSTORE_H
#define SETTINGSSTORE_H

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
#include "core/containersstruct.h"
#include "core/sqlite.h"

class SettingsStore {
 public:
  ~SettingsStore();
  QVariant read(const QString& k);
  QList<containerProperties> read();
  void write(const QString& k, const QVariant& d);
  void write(const QList<containerProperties>& cp);
  QString settingsPath();
  const QString c = "CCLC";
  const QString a = "ASW";

 private:
  const QString sfn = "selection.json";
  QSettings* s =
      new QSettings(QSettings::IniFormat, QSettings::UserScope, c, a);
  QJsonObject toJson(const containerProperties& cp);
  containerProperties toContainerStruct(const QJsonObject& obj);
};

#endif  // SETTINGSSTORE_H
