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
 * @class Basis
 * @brief Provides methods for intra-component work.
 * @details Basis does a few important things:
 *   - provides identifiers for saving settings
 *   - reads and writes a list of sources, settings and message history
 *   - stores a list of sources in memory
 *   - checks the settings file for errors.
 * @sa SQLite, QSettings
 */
class Basis : public QObject {
  Q_OBJECT
public:
  // Objects:
  SQLite *sql = new SQLite(this); /*!< SQLite handler. */

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
  Basis(QObject *parent = nullptr) : QObject(parent) { read_source_list(); }

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
   * @fn Basis::get_sources
   * @returns a list of sources @a _sources.
   */
  QList<Source> get_sources() { return _sources; }

  // Functions described in `basis.cpp`:
  void read_source_list();
  void check_settings_file();
  QList<Message> read_message_history(QFile *file);
  void write(const QString &key, const QVariant &data);
  void write_source_list(QList<Source> source_list);
  void write_message_history(QList<Message> message_history, QFile *file);

signals:
  /*!
   * @brief Reports an error while parsing JSON.
   */
  QString json_error(QString error_text);

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

  // Constants:
  inline static const QString sources_store_filename = "sources.json";

  // Functions described in `basis.cpp`:
  QJsonArray read_json(QFile *file);
  void write_json(QFile *savefile, QJsonArray json_array);
  QJsonObject to_json(const Source &source);
  QJsonObject to_json(const Message &message);
  Source to_source(const QJsonObject &json_object);
  Message to_message(const QJsonObject &json_object);
};

#endif // BASIS_H
