#ifndef BASIS_H
#define BASIS_H

#define sccc static constexpr const char *

#include "core-kit/database/json.hpp"
#include "core-kit/database/sqlite.hpp"
#include "core-kit/extensions/scenario.hpp"
#include "core-kit/extensions/script.hpp"
#include "core-kit/model/keystore.hpp"
#include "core-kit/model/nlp/cacher.hpp"
#include "core-kit/model/source.hpp"
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QIODevice>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QList>
#include <QPair>
#include <QSettings>
#include <QString>
#include <QTextStream>
#include <QVariant>

/*! @class Basis
 *  @brief Provides methods for intra-component work.
 *  @details Basis does a lot of important things:
 *   - provides identifiers for saving settings
 *   - reads and writes a list of sources, settings and message history
 *   - stores a list of sources and cached expressions
 *   - checks the settings file for errors
 *   - stores context and memory map.
 *  @sa SQLite, Json, PythonModule, NLPmodule, QSettings  */
class Basis : public QObject {
  Q_OBJECT
  Q_DISABLE_COPY(Basis)
public:
  // Constants:
  sccc companyName     = "cclc";
  sccc applicationName = "jeff";

  sccc isMenuBarHiddenSt = "jeff-qt/menubarishidden";
  sccc posSt             = "jeff-qt/pos";
  sccc sizeSt            = "jeff-qt/size";
  sccc isFullScreenSt    = "jeff-qt/isfullscreen";
  sccc isNotFirstStartSt = "jeff-qt/isnotfirststart";
  sccc helpTipSt         = "jeff-qt/helptip";

  sccc isMonologueEnabledSt          = "core-kit/ismonologuemodeenabled";
  sccc isDelayEnabledSt              = "core-kit/isdelayenabled";
  sccc minDelaySt                    = "core-kit/mindelay";
  sccc maxDelaySt                    = "core-kit/maxdelay";
  sccc isHistoryKeepingEnabledSt     = "core-kit/ishistorykeepingenabled";
  sccc isGreetingsEnabledSt          = "core-kit/isgreetingsenabled";
  sccc greetingsMsg                  = "core-kit/greetings";
  sccc scenarioExitMsg               = "core-kit/scenarioexitter";
  sccc serverPortSt                  = "core-kit/serverport";
  sccc extensionKillSecSt            = "core-kit/extensionkillsec";
  sccc seacrhInDbWhenRepeated        = "core-kit/repeatedexplicitindb";
  sccc disableMessagesTransmissionSt = "core-kit/nomessagetransmission";

  sccc defaultSourcePath      = "sources/defaultsourcepath";
  sccc defaultSourceContainer = "sources/defaultsourcecontainer";

  sccc serverAuthorizationOnSt = "server/authorize";
  sccc serverAuthKeySt         = "server/auth_key";

  sccc customScannerSt    = "scripts/custom_scanner";
  sccc customScannerFnSt  = "scripts/custom_scanner_fn_name";
  sccc customComposerSt   = "scripts/custom_composer";
  sccc customComposerFnSt = "scripts/custom_composer_fn_name";
  sccc customComposerSASt = "scripts/custom_composer_send_adprops";

  sccc scriptPathWk       = "path";
  sccc funcNameWk         = "fn_name";
  sccc getHistWk          = "hist_parts";
  sccc errorTypeWk        = "error_type";
  sccc recentMessagesWk   = "recent_messages";
  sccc readMemoryWk       = "memory_cells";
  sccc memoryValuesWk     = "memory_values";
  sccc contextWk          = "context";
  sccc exprPropsWk        = "expression_properties";
  sccc writeMemoryWk      = "store_in_memory";
  sccc scenarioReadyWk    = "sready";
  sccc scenarioQueuedWk   = "squeued";
  sccc scenarioAddrWk     = "saddr";
  sccc scenarioPortWk     = "sport";
  sccc scenarioNameWk     = "sname";
  sccc scenarioTokenWk    = "stoken";
  sccc scenarioContinueWk = "scontinue";
  sccc scenarioFinishWk   = "sfinish";
  sccc sendWk             = "send";
  sccc selectionWk        = "selection";
  sccc searchAgainWk      = "search_again";
  sccc sendAsUserWk       = "send_as_user";
  sccc sendStatusWk       = "send_status";
  sccc sendWarningWk      = "send_warning";
  sccc sendInfoWk         = "send_info";
  sccc authKeyWk          = "auth_key";
  sccc addExpressionWk    = "add_expr";

#ifdef JEFF_WITH_QT_WIDGETS
  sccc source_manager_cmd    = "/sources";
  sccc script_manager_cmd    = "/scripts";
  sccc first_start_cmd       = "/first";
  sccc about_cmd             = "/about";
  sccc help_cmd              = "/help";
  sccc settings_cmd          = "/settings";
  sccc phrase_editor_cmd     = "/phrases";
  sccc extensions_viewer_cmd = "/extensions";
#endif
  sccc fast_append_cmd        = "/+ ";
  sccc fast_append_script_cmd = "/++ ";
  sccc appeal_cmd             = "@";
  sccc monologue_mode_cmd     = "/mm";
  sccc add_scanner_cmd        = "/+s";
  sccc add_composer_cmd       = "/+c";
  sccc stop_scanner_cmd       = "/-s";
  sccc stop_composer_cmd      = "/-c";
  
  sccc bundle_dir_name = "bundle";
  
  // Objects:
  Json *json = nullptr;     /*!< Json handler.   */
  SQLite *sql = nullptr;    /*!< SQLite handler. */
  Cacher *cacher = nullptr; /*!< Cache handler.  */
  ScriptMeta *custom_scanner = nullptr;
  ScriptMeta *custom_composer = nullptr;

  // Functions:
  /*! @brief Determines if a settings file exists. */
  inline bool exists() { return QFile::exists(_settings.fileName()); }
  /*! @brief Determines if a database specified by @a filename exists. */
  inline bool db_exists(const QString &filename) { return QFile::exists(filename); }
  /*! @brief Determines whether the settings file is readable or writable. */
  inline bool accessible() { return _settings.status() != QSettings::AccessError; }
  /*! @brief Determines the correctness of the settings file format. */
  inline bool correct() { return _settings.status() != QSettings::FormatError; }
  /*! @brief Clears scenario's token. */
  inline void clear_stoken() { _scenario_token = QString(); }
  /*! @brief Sets scenario's token. */
  inline void set_stoken(const QString &token) { _scenario_token = token; }

  /*! @brief Reads the setting. */
  inline QVariant read(const QString &key)       { return _settings.value(key); }
  inline QVariant operator[](const char *key)    { return _settings.value(key); }
  inline QVariant operator[](const QString &key) { return read(key); }

  /*! @brief Says if there is a parameter with this key in the settings. */
  inline bool contains(const char *key) { return _settings.contains(key); }
  /*! @brief Determines where application settings are stored. */
  inline QString get_settings_path() { return QFileInfo(_settings.fileName()).absolutePath(); }

  // Functions described in `basis.cpp`:
  Basis(QObject *parent = nullptr);
  ~Basis();
  void write(const QString &key, const QVariant &data);
  void check_settings_file();
  void check_default_source();
  void load_sources();
  void load_memory();
  void save_memory();
  Sources sources();
  void sources(Sources s);
  void memory(const QString &key, QJsonValue data);
  QJsonValue memory(const QString &key);
  QJsonObject handle_to_script(const QJsonObject &object);
  void handle_from_script(const QJsonObject &object, bool except_send = false);
  void warn_about(QString warning_text);
  void set_custom_scanner(ScriptMeta *_custom_scanner);
  void set_custom_composer(ScriptMeta *_custom_composer);

signals:
  /*! @brief Reports some information. */
  QString info(QString info_text);
  /*! @brief Reports about a warning. */
  QString warn(QString warning_text);
  /*! @brief Notifies of a message from a script that should be shown on the screen. */
  QString send(QString outter_message);
  /*! @brief Notifies of the need to repeat the search for the specified input. */
  QString search_again(QString rephrased_message);
  /*! @brief Notifies of a message from a script that should be shown on the screen as a user message
   *  (on the right side of the screen).  */
  QString send_as_user(QString outter_message);
  /*! @brief Notifies of a message from a script that should be displayed on the screen and may change over time. */
  QPair<QString, QString> send_status(QPair<QString, QString> id_and_message);
  /*! @brief Reports that sources has been changed. */
  void sources_changed();
  /*! @brief Starts a scenario. @sa Core */
  ScenarioServerMeta start_scenario(ScenarioServerMeta scenario_meta);
  /*! @brief Adds a script to the queue. */
  ScenarioServerMeta schedule_scenario(ScenarioServerMeta scenario_meta);
  /*! @brief Stops current scenario. */
  void shutdown_scenario();
  /*! @brief Notifies that the custom scanner has been changed. */
  void custom_scanner_changed();
  /*! @brief Notifies that the custom composer has been changed. */
  void custom_composer_changed();

private:
  // Objects:
  Sources _sources = Sources();    /*!< List of sources for @a JCK.  */
  QString _scenario_token = "";    /*!< Token for scenarios.     */
  KeyStore _memory = KeyStore();   /*!< Long-life memory.    */

  /*! Qt settings object. */
  QSettings _settings = QSettings(QSettings::IniFormat, QSettings::UserScope, companyName, applicationName);

  // Functions described in `basis.cpp`:
  void set_first_source_as_default();
  Source get_default_source();
};

#endif
