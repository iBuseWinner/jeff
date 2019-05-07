#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QIODevice>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QList>
#include <QSettings>
#include <QString>
#include <QTextStream>
#include <QVariant>
#include "core/container.h"
#include "core/message.h"
#include "core/sqlite.h"

class settings : public QObject {
  Q_OBJECT
 public:
  settings(QObject* p = nullptr);
  const QString c = "CCLC";
  const QString a = "ASW";
  QVariant read(const QString& k);
  QList<container> read();
  QList<message> read(QFile* sf);
  void write(const QString& k, const QVariant& d);
  void write(QList<container> cp);
  void write(QList<message> mh, QFile* sf);
  QString settingsPath();

 signals:
  QString jsonError(QString et);
  QString roWarning(QString wt = "Settings file is read only.");

 private:
  const QString sfn = "selection.json";
  QSettings* s =
      new QSettings(QSettings::IniFormat, QSettings::UserScope, c, a, this);
  QJsonArray r(QFile* sf);
  QJsonObject toJ(const container& cProp);
  QJsonObject toJ(const message& daemon);
  container toC(const QJsonObject& obj);
  message toM(const QJsonObject& obj);
};

#endif  // SETTINGS_H
