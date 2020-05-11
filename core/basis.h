#ifndef BASIS_H
#define BASIS_H

#include "core/source.h"
#include "core/message.h"
#include "core/sqlite.h"
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

/*!
 * Class: CoreMethods.
 * Provides methods for intra-component work.
 */
class Basis : public QObject {
  Q_OBJECT
public:
  // Objects:
  SQLite *sql = new SQLite(this);
  const QString companyName = "CCLC";
  const QString applicationName = "ASW";

  const QString isMenuBarHiddenSt = "asw/menubarishidden";
  const QString sizeSt = "asw/size";
  const QString isFullScreenSt = "asw/isfullscreen";
  const QString isNotFirstStartSt = "asw/isnotfirststart";

  const QString isMonologueModeEnabledSt = "core/ismonologuemodeenabled";
  const QString isDelayEnabledSt = "core/isdelayenabled";
  const QString minDelaySt = "core/mindelay";
  const QString maxDelaySt = "core/maxdelay";
  const QString isKeepingEnabledSt = "core/iskeepingenabled";
  const QString isHintsEnabledSt = "core/ishintsenabled";

  // Functions:
  Basis(QObject *parent = nullptr);
  QList<Source> readSourceList();
  QList<message> readMessageHistory(QFile *file);
  void write(const QString &key, const QVariant &data);
  void writeSourceList(QList<Source> sourceList);
  void writeMessageHistory(QList<message> messageHistory, QFile *sf);

  /*! Returns whether the settings file exists. */
  bool exists() { return QFile::exists(s->fileName()); }
  /*! There is no access to the settings file? */
  bool isUnaccessed() { return !access; }
  /*! The file is incorrect? */
  bool isIncorrect() { return !correct; }
  /*! Reads the setting. */
  QVariant read(const QString &key) { return s->value(key); }
  /*! Returns the path to the settings. */
  QString settingsPath() { return QFileInfo(s->fileName()).absolutePath(); }

signals:
  QString jsonError(QString errorText);
  QString settingsWarning(QString warningText);

private:
  // Objects:
  bool access = true;
  bool correct = true;
  const QString cfn = "sources.json";
  QSettings *s = new QSettings(QSettings::IniFormat, QSettings::UserScope,
                               companyName, applicationName, this);
  QList<Source> cProps;

  // Functions:
  QJsonArray readJson(QFile *f);
  void writeJson(QFile *sf, QJsonArray arr);
  QJsonObject toJSON(const Source &cProp);
  QJsonObject toJSON(const message &shadow);
  Source toSource(const QJsonObject &obj);
  message toMessage(const QJsonObject &obj);
};

#endif // BASIS_H
