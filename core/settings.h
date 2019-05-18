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

/*!
 * Class: settings.
 * Contains methods for working with program settings.
 */
class settings : public QObject {
  Q_OBJECT
 public:
  // Objects:
  sqlite* SQL = new sqlite(this);
  const QString companyName = "CCLC";
  const QString applicationName = "ASW";

  // Functions:
  settings(QObject* parent = nullptr);
  QList<container> readContainerList();
  QList<message> readMessageHistory(QFile* file);
  void write(const QString& key, const QVariant& data);
  void writeContainerList(QList<container> containerList);
  void writeMessageHistory(QVector<message> messageHistory, QFile* sf);

  /*! Returns whether the settings file exists. */
  bool exists() { return QFile::exists(s->fileName()); }
  /*! There is no access to the settings file? */
  bool isUnaccessed() { return !access; }
  /*! The file is incorrect? */
  bool isIncorrect() { return !correct; }
  /*! Reads the setting. */
  QVariant read(const QString& key) { return s->value(key); }
  /*! Returns the path to the settings. */
  QString settingsPath() { return QFileInfo(s->fileName()).absolutePath(); }

 signals:
  QString jsonError(QString errorText);
  QString settingsWarning(QString warningText);

 private:
  // Objects:
  bool access = true;
  bool correct = true;
  const QString cfn = "containers.json";
  QSettings* s = new QSettings(QSettings::IniFormat, QSettings::UserScope,
                               companyName, applicationName, this);

  // Functions:
  QJsonArray readJson(QFile* f);
  void writeJson(QFile* sf, QJsonArray arr);
  QJsonObject toJ(const container& cProp);
  QJsonObject toJ(const message& shadow);
  container toC(const QJsonObject& obj);
  message toM(const QJsonObject& obj);
};

#endif  // SETTINGS_H
