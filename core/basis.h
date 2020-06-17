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
  inline static const QString companyName = "CCLC";
  inline static const QString applicationName = "ASW";

  inline static const QString isMenuBarHiddenSt = "asw/menubarishidden";
  inline static const QString sizeSt = "asw/size";
  inline static const QString isFullScreenSt = "asw/isfullscreen";
  inline static const QString isNotFirstStartSt = "asw/isnotfirststart";

  inline static const QString isMonologueModeEnabledSt =
      "core/ismonologuemodeenabled";
  inline static const QString isDelayEnabledSt = "core/isdelayenabled";
  inline static const QString minDelaySt = "core/mindelay";
  inline static const QString maxDelaySt = "core/maxdelay";
  inline static const QString isKeepingEnabledSt = "core/iskeepingenabled";
  inline static const QString isHintsEnabledSt = "core/ishintsenabled";

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
