#ifndef JSON_H
#define JSON_H

#include "core-kit/database/sqlite.hpp"
#include "core-kit/extensions/extension.hpp"
#include "core-kit/model/keystore.hpp"
#include "core-kit/model/message.hpp"
#include "core-kit/model/nlp/cache.hpp"
#include "core-kit/model/source.hpp"
#include "yelloger.h"
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QList>

/*! @class Json
 *  @brief Provides methods for reading and writing data in/from json files.  */
class Json : public QObject {
  Q_OBJECT
  Q_DISABLE_COPY(Json)
public:
  // Constants:
  static constexpr const char *sources_store_filename    = "sources.json"    ;
  static constexpr const char *cache_store_filename      = "nlp_cache.json"  ;
  static constexpr const char *history_store_filename    = "history.json"    ;
  static constexpr const char *extensions_store_filename = "extensions.json" ;
  static constexpr const char *memory_store_filename     = "memory.json"     ;
  static constexpr const char *log_filename              = "log.txt"         ;
  static constexpr const char *subdir_name               = "jeff-qt"         ;
  
  // Functions described in `json.cpp`:
  Json(QString settingsPath, QObject *parent = nullptr);
  Sources  read_source_list         (SQLite *sql) ;
  MessagesMeta read_message_history (QFile *file) ;
  MessagesMeta read_message_history ()            ;
  Cache    read_NLP_cache           ()            ;
  ExtensionsMeta  read_extensions   ()            ;
  KeyStore read_memory              ()            ;
  void write_source_list     (SQLite *sql, Sources sources)              ;
  void write_message_history (MessagesMeta message_history, QFile *file) ;
  void write_message_history (MessagesMeta message_history)              ;
  void write_NLP_cache       (Cache cache)                               ;
  void write_extensions      (ExtensionsMeta scripts)                    ;
  void write_memory          (KeyStore memory)                           ;

private:
  // Objects:
  QString _settings_path;

  // Functions described in `json.cpp`:
  void check_or_create_subdir();
  QJsonArray read_json(QFile *file);
  void write_json(QFile *savefile, const QJsonArray &json_array);
};

#endif
