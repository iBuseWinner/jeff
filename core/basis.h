#ifndef BASIS_H
#define BASIS_H

#include "core/database/json.h"
#include "core/database/sqlite.h"
#include "core/model/message.h"
#include "core/model/nlp/cacher.h"
#include "core/model/source.h"
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QIODevice>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QList>
#include <QMutex>
#include <QSettings>
#include <QString>
#include <QTextStream>
#include <QVariant>

/*!
 * @class Basis
 * @brief Provides methods for intra-component work.
 * @details Basis does a few important things:
 *   - provides identifiers for saving settings
 *   - reads and writes a list of sources, settings and message history
 *   - stores a list of sources in memory
 *   - checks the settings file for errors.
 * @sa SQLite, Json, QSettings
 */
class Basis : public QObject {
  Q_OBJECT
public:
  // Objects:
  SQLite *sql = new SQLite(this);    /*!< SQLite handler. */
  Cacher *cacher = new Cacher(this); /*!< Cache handler.  */
  Json *json = nullptr;              /*!< Json handler.   */

  // Constants:
  const char *companyName = "CCLC";
  const char *applicationName = "Jeff";

  const char *isMenuBarHiddenSt = "jeff-qt/menubarishidden";
  const char *sizeSt = "jeff-qt/size";
  const char *isFullScreenSt = "jeff-qt/isfullscreen";
  const char *isNotFirstStartSt = "jeff-qt/isnotfirststart";

  const char *isMonologueModeEnabledSt = "core/ismonologuemodeenabled";
  const char *isDelayEnabledSt = "core/isdelayenabled";
  const char *minDelaySt = "core/mindelay";
  const char *maxDelaySt = "core/maxdelay";
  const char *isKeepingEnabledSt = "core/iskeepingenabled";
  const char *isHintsEnabledSt = "core/ishintsenabled";
  const char *isInaccurateSearchEnabledSt = "core/isinnacuratesearchenabled";
  
  const char *defaultSourcePath = "sources/defaultsourcepath";
  const char *defaultSourceContainer = "sources/defaultsourcecontainer";

  // Functions:
  /*!
   * @fn Basis::Basis
   * @brief The constructor.
   * @details At initialization, it reads a list of sources from @a sourcesStoreFilename file.
   * @param[in,out] parent QObject parent
   */
  Basis(QObject *parent = nullptr) : QObject(parent) {
    json = new Json(get_settings_path(), this);
    load_sources();
  }

  /*!
   * @fn Basis::exists
   * @brief Determines if a settings file exists.
   * @returns the boolean value of the existence of the file.
   */
  inline bool exists() { return QFile::exists(_settings.fileName()); }
  
  /*!
   * @fn Basis::db_exists
   * @brief Determines if a database specified by @a filename exists.
   * @param[in] filename
   * @returns the boolean value of existence of the file.
   */
  inline bool db_exists(const QString &filename) { return QFile::exists(filename); }

  /*!
   * @fn Basis::accessible
   * @brief Determines whether the settings file is readable or writable.
   * @returns the boolean value of the accessibility of the settings file.
   */
  inline bool accessible() { return _settings.status() != QSettings::AccessError; }

  /*!
   * @fn Basis::correct
   * @brief Determines the correctness of the settings file format.
   * @returns the boolean value of the correctness of the settings file.
   */
  inline bool correct() { return _settings.status() != QSettings::FormatError; }

  /*!
   * @fn Basis::read
   * @brief Reads the setting.
   * @param[in] key a key to get the value
   * @returns value for @a key.
   */
  inline QVariant read(const QString &key)       { return _settings.value(key); }
  inline QVariant operator[](const char *key)    { return _settings.value(key); }
  inline QVariant operator[](const QString &key) { return read(key); }
  
  /*! Says if there is a parameter with this key in the settings. */
  inline bool contains(const char *key) { return _settings.contains(key); }

  /*!
   * @fn Basis::get_settings_path
   * @brief Determines where application settings are stored.
   * @returns the absolute path of the application settings folder.
   */
  inline QString get_settings_path() { return QFileInfo(_settings.fileName()).absolutePath(); }

  /*!
   * @fn Basis::load_sources
   * @brief Loads @a _sources from file.
   */
  void load_sources() {
    sources_mutex.lock();
    if (not sources.isEmpty()) sources.clear();
    Sources tmp = json->read_source_list(sql);
    for (int i = 0; i < tmp.length(); i++) {
      if (db_exists(tmp[i].path))
        if (not sources.contains(tmp[i]))
          sources.append(tmp[i]);
    }
    sources_mutex.unlock();
  }

  /*!
   * @fn Basis::get_sources
   * @returns a list of sources @a _sources.
   */
  inline Sources get_sources() {
    sources_mutex.lock();
    Sources s = sources;
    sources_mutex.unlock();
    return s;
  }

  /*!
   * @fn Basis::set_sources
   * @brief Saves @a sources list.
   */
  void set_sources(Sources _sources) {
    sources_mutex.lock();
    sources = _sources;
    sources_mutex.unlock();
    json->write_source_list(sql, _sources);
    check_default_source();
  }

  // Functions described in `basis.cpp`:
  void check_settings_file();
  void check_default_source();
  void write(const QString &key, const QVariant &data);

signals:
  /*!
   * @brief Reports a result of checking the settings file, if it is incorrect.
   */
  QString settings_warning(QString warning_text);

private:
  // Objects:
  QMutex sources_mutex, context_mutex;
  Sources sources;      /*!< List of sources for @a NLPmodule. */
  Options context;      /*!< Context values (@sa AIML). */
  
  /*! Qt settings object. */
  QSettings _settings = QSettings(
    QSettings::IniFormat, QSettings::UserScope, companyName, applicationName
  );
  
  // Functions described in `basis.cpp`:
  void set_first_source_as_default();
};

#endif
