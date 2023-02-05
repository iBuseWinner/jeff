#ifndef BASIS_H
#define BASIS_H

#include "core-kit/database/json.hpp"
#include "core-kit/database/sqlite.hpp"
#include "core-kit/model/keystore.hpp"
#include "core-kit/model/message.hpp"
#include "core-kit/model/nlp/cacher.hpp"
#include "core-kit/model/nlp/options.hpp"
#include "core-kit/model/source.hpp"
#include "yelloger.h"
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
#include <QPair>
#include <QSettings>
#include <QString>
#include <QTextStream>
#include <QVariant>

/*! @brief The address and port of the server responsible for the specific scenario. */
struct ScenarioServerMeta {
  QHostAddress server_addr;
  quint16 server_port;
  QString auth_key;
};

/*!
 * @class Basis
 * @brief Provides methods for intra-component work.
 * @details Basis does a lot of important things:
 *   - provides identifiers for saving settings
 *   - reads and writes a list of sources, settings and message history
 *   - stores a list of sources and cached expressions
 *   - checks the settings file for errors
 *   - stores context and memory map.
 * @sa SQLite, Json, PythonModule, NLPmodule, QSettings
 */
class Basis : public QObject {
  Q_OBJECT
  Q_DISABLE_COPY(Basis)
public:
  // Constants:
  static constexpr const char *companyName     = "cclc";
  static constexpr const char *applicationName = "jeff";

  static constexpr const char *isMenuBarHiddenSt = "jeff-qt/menubarishidden";
  static constexpr const char *sizeSt            = "jeff-qt/size";
  static constexpr const char *isFullScreenSt    = "jeff-qt/isfullscreen";
  static constexpr const char *isNotFirstStartSt = "jeff-qt/isnotfirststart";

  static constexpr const char *isMonologueEnabledSt          = "core-kit/ismonologuemodeenabled";
  static constexpr const char *isDelayEnabledSt              = "core-kit/isdelayenabled";
  static constexpr const char *minDelaySt                    = "core-kit/mindelay";
  static constexpr const char *maxDelaySt                    = "core-kit/maxdelay";
  static constexpr const char *isHistoryKeepingEnabledSt     = "core-kit/ishistorykeepingenabled";
  static constexpr const char *isGreetingsEnabledSt          = "core-kit/isgreetingsenabled";
  static constexpr const char *greetingsMsg                  = "core-kit/greetings";
  static constexpr const char *scenarioExitMsg               = "core-kit/scenarioexitter";
  static constexpr const char *serverPortSt                  = "core-kit/serverport";
  static constexpr const char *extensionKillSecSt            = "core-kit/extensionkillsec";
  static constexpr const char *seacrhInDbWhenRepeated        = "core-kit/repeatedexplicitindb";
  static constexpr const char *disableMessagesTransmissionSt = "core-kit/nomessagetransmission";

  static constexpr const char *defaultSourcePath      = "sources/defaultsourcepath";
  static constexpr const char *defaultSourceContainer = "sources/defaultsourcecontainer";

  static constexpr const char *serverAuthorizationOnSt = "server/authorize";
  static constexpr const char *serverAuthKeySt         = "server/auth_key";

  static constexpr const char *customScannerSt    = "scripts/custom_scanner";
  static constexpr const char *customScannerFnSt  = "scripts/custom_scanner_fn_name";
  static constexpr const char *customComposerSt   = "scripts/custom_composer";
  static constexpr const char *customComposerFnSt = "scripts/custom_composer_fn_name";
  static constexpr const char *customComposerSASt = "scripts/custom_composer_send_adprops";

  static constexpr const char *scriptPathWk       = "path";
  static constexpr const char *funcNameWk         = "fn_name";
  static constexpr const char *getHistWk          = "hist_parts";
  static constexpr const char *errorTypeWk        = "error_type";
  static constexpr const char *recentMessagesWk   = "recent_messages";
  static constexpr const char *readMemoryWk       = "memory_cells";
  static constexpr const char *memoryValuesWk     = "memory_values";
  static constexpr const char *contextWk          = "context";
  static constexpr const char *exprPropsWk        = "expression_properties";
  static constexpr const char *writeMemoryWk      = "store_in_memory";
  static constexpr const char *scenarioReadyWk    = "sready";
  static constexpr const char *scenarioAddrWk     = "saddr";
  static constexpr const char *scenarioPortWk     = "sport";
  static constexpr const char *scenarioTokenWk    = "stoken";
  static constexpr const char *scenarioContinueWk = "scontinue";
  static constexpr const char *scenarioFinishWk   = "sfinish";
  static constexpr const char *sendWk             = "send";
  static constexpr const char *selectionWk        = "selection";
  static constexpr const char *searchAgainWk      = "search_again";
  static constexpr const char *sendAsUserWk       = "send_as_user";
  static constexpr const char *sendStatusWk       = "send_status";
  static constexpr const char *sendWarningWk      = "send_warning";
  static constexpr const char *authKeyWk          = "auth_key";

#ifdef JEFF_WITH_QT_WIDGETS
  static constexpr const char *source_manager_cmd    = "/sources";
  static constexpr const char *script_manager_cmd    = "/scripts";
  static constexpr const char *first_start_cmd       = "/first";
  static constexpr const char *about_cmd             = "/about";
  static constexpr const char *help_cmd              = "/help";
  static constexpr const char *settings_cmd          = "/settings";
  static constexpr const char *phrase_editor_cmd     = "/phrases";
  static constexpr const char *extensions_viewer_cmd = "/extensions";
#endif
  static constexpr const char *fast_append_cmd        = "/+ ";
  static constexpr const char *fast_append_script_cmd = "/++ ";
  static constexpr const char *monologue_mode_cmd     = "/mm";
  static constexpr const char *stop_scanner_cmd       = "/-s";
  static constexpr const char *stop_composer_cmd      = "/-c";
  static constexpr const char *add_scanner_cmd        = "/+s";
  static constexpr const char *add_composer_cmd       = "/+c";
  
  // Objects:
  Json *json = nullptr;     /*!< Json handler.   */
  SQLite *sql = nullptr;    /*!< SQLite handler. */
  Cacher *cacher = nullptr; /*!< Cache handler.  */

  // Functions:
  /*! @brief The constructor. */
  Basis(QObject *parent = nullptr) : QObject(parent) {
    json = new Json(get_settings_path(), this); /*!< @details Json object will be created first 'cause it inits yelloger. */
    sql = new SQLite(this);
    cacher = new Cacher(this);
    load_sources();
    load_memory();
  }

  /*! @brief The destructor. */
  ~Basis() { save_memory(); }

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

  /*! @brief Reads the setting. */
  inline QVariant read(const QString &key)       { return _settings.value(key); }
  inline QVariant operator[](const char *key)    { return _settings.value(key); }
  inline QVariant operator[](const QString &key) { return read(key); }

  /*! @brief Says if there is a parameter with this key in the settings. */
  inline bool contains(const char *key) { return _settings.contains(key); }
  /*! @brief Determines where application settings are stored. */
  inline QString get_settings_path() { return QFileInfo(_settings.fileName()).absolutePath(); }

  // Functions described in `basis.cpp`:
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

signals:
  /*! @brief Reports about something. */
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
  /*! @brief Sends last message to scenario. @sa Core */
  ScenarioServerMeta start_scenario(ScenarioServerMeta scenario_meta);
  /*! @brief Stops current scenario. */
  void shutdown_scenario();

private:
  // Objects:
  Sources _sources = Sources();   /*!< List of sources for @a JCK. */
  KeyStore _memory = KeyStore();  /*!< Long-life memory. */
  QString _scenario_token = "";   /*!< Token for scenarios. */

  /*! Qt settings object. */
  QSettings _settings = QSettings(QSettings::IniFormat, QSettings::UserScope, companyName, applicationName);

  // Functions described in `basis.cpp`:
  void set_first_source_as_default();
};

#endif
