#ifndef BASIS_H
#define BASIS_H

#include "core/database/json.h"
#include "core/database/sqlite.h"
#include "core/model/message.h"
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
  SQLite *sql = new SQLite(this); /*!< SQLite handler. */
  Json *json = new Json(this);    /*!< Json handler. */

  // Constants:
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
  /*!
   * @fn Basis::Basis
   * @brief The constructor.
   * @details At initialization, it reads a list of sources from the @a
   * sourcesStoreFilename file.
   * @param[in,out] parent QObject parent
   */
  Basis(QObject *parent = nullptr) : QObject(parent) { load_sources(); }

  /*!
   * @fn Basis::exists
   * @brief Determines if a settings file exists.
   * @returns the boolean value of the existence of the file.
   */
  bool exists() { return QFile::exists(_settings->fileName()); }

  /*!
   * @fn Basis::accessible
   * @brief Determines whether the settings file is readable or writable.
   * @returns the boolean value of the accessibility of the settings file.
   */
  bool accessible() { return _settings->status() != QSettings::AccessError; }

  /*!
   * @fn Basis::correct
   * @brief Determines the correctness of the settings file format.
   * @returns the boolean value of the correctness of the settings file.
   */
  bool correct() { return _settings->status() != QSettings::FormatError; }

  /*!
   * @fn Basis::read
   * @brief Reads the setting.
   * @param[in] key a key to get the value
   * @returns value for @a key.
   */
  QVariant read(const QString &key) { return _settings->value(key); }
  QVariant operator[](const QString &key) { return _settings->value(key); }

  /*!
   * @fn Basis::get_settings_path
   * @brief Determines where application settings are stored.
   * @returns the absolute path of the application settings folder.
   */
  QString get_settings_path() {
    return QFileInfo(_settings->fileName()).absolutePath();
  }

  /*!
   * @fn Basis::load_sources
   * @brief Loads @a _sources from file.
   */
  void load_sources() {
    for (auto source : json->read_source_list(sql, get_settings_path())) {
      if (not _sources.contains(source))
        _sources.append(source);
    }
  }

  /*!
   * @fn Basis::get_sources
   * @returns a list of sources @a _sources.
   */
  QList<Source> get_sources() { return _sources; }

  /*!
   * @fn Basis::set_sources
   * @brief Saves @a sources list.
   */
  void set_sources(QList<Source> sources) {
    _sources = sources;
    json->write_source_list(sql, get_settings_path(), sources);
  }

  // Functions described in `basis.cpp`:
  void check_settings_file();
  void write(const QString &key, const QVariant &data);

signals:
  /*!
   * @brief Reports a result of checking the settings file, if it is incorrect.
   */
  QString settings_warning(QString warning_text);

private:
  // Objects:
  QSettings *_settings =
      new QSettings(QSettings::IniFormat, QSettings::UserScope, companyName,
                    applicationName, this); /*!< Qt settings object. */
  QList<Source> _sources; /*!< List of sources for @a NLPmodule. */
};

#endif // BASIS_H
