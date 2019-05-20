#ifndef CORE_METHODS_H
#define CORE_METHODS_H

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
 * Class: CoreMethods.
 * Provides methods for intra-component work.
 */
class CoreMethods : public QObject {
  Q_OBJECT
 public:
  // Objects:
  sqlite* SQL = new sqlite(this);
  const QString companyName = "CCLC";
  const QString applicationName = "ASW";
  const QString isMenuBarHiddenSt = "asw/menubarishidden";
  const QString sizeSt = "asw/size";
  const QString isFullScreenSt = "asw/isfullscreen";
  const QString isNotFirstStartSt = "asw/isnotfirststart";
  const QString isMonologueEnabledSt = "core/ismonologueenabled";
  const QString isDelayEnabledSt = "core/isdelayenabled";
  const QString minDelaySt = "core/mindelay";
  const QString maxDelaySt = "core/maxdelay";
  const QString isKeepingEnabledSt = "core/iskeepingenabled";
  const QString isHintsEnabledSt = "core/ishintsenabled";

  // Functions:
  CoreMethods(QObject* parent = nullptr);
  QList<container> readContainerList();
  QList<message> readMessageHistory(QFile* file);
  void write(const QString& key, const QVariant& data);
  void writeContainerList(QList<container> containerList);
  void writeMessageHistory(QList<message> messageHistory, QFile* sf);

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
  QList<container> cProps;

  // Functions:
  QJsonArray readJson(QFile* f);
  void writeJson(QFile* sf, QJsonArray arr);
  QJsonObject toJSON(const container& cProp);
  QJsonObject toJSON(const message& shadow);
  container toContainer(const QJsonObject& obj);
  message toMessage(const QJsonObject& obj);
};

#endif  // CORE_METHODS_H
