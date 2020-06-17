#ifndef BASIS_H
#define BASIS_H

#include "core/message.h"
#include "core/source.h"
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
 * Class: Basis.
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
  Basis(QObject *parent = nullptr) : QObject(parent) { readSourceList(); }
  void readSourceList();
  void check();
  QList<Message> readMessageHistory(QFile *file);
  void write(const QString &key, const QVariant &data);
  void writeSourceList(QList<Source> sourceList);
  void writeMessageHistory(QList<Message> message_history, QFile *file);

  /*! Returns whether the settings file exists. */
  bool exists() { return QFile::exists(settings->fileName()); }
  /*! There is no access to the settings file? */
  bool isUnaccessed() { return !access; }
  /*! The file is incorrect? */
  bool isIncorrect() { return !correct; }
  /*! Reads the setting. */
  QVariant read(const QString &key) { return settings->value(key); }
  /*! Returns the path to the settings. */
  QString settingsPath() {
    return QFileInfo(settings->fileName()).absolutePath();
  }
  /*! Returns list of sources. */
  QList<Source> getSources() { return sources; }

signals:
  QString jsonError(QString errorText);
  QString settingsWarning(QString warningText);

private:
  // Objects:
  bool access = true;
  bool correct = true;
  inline static const QString sourcesStoreFilename = "sources.json";
  QSettings *settings =
      new QSettings(QSettings::IniFormat, QSettings::UserScope, companyName,
                    applicationName, this);
  QList<Source> sources;

  // Functions:
  QJsonArray readJson(QFile *file);
  void writeJson(QFile *savefile, QJsonArray jsonArray);
  QJsonObject toJSON(const Source &source);
  QJsonObject toJSON(const Message &message);
  Source toSource(const QJsonObject &jsonObject);
  Message toMessage(const QJsonObject &jsonObject);
};

#endif // BASIS_H
