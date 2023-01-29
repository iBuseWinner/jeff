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
  // Constants:
  const char *sources_store_filename    = "sources.json"    ;
  const char *cache_store_filename      = "nlp_cache.json"  ;
  const char *history_store_filename    = "history.json"    ;
  const char *extensions_store_filename = "extensions.json" ;
  const char *memory_store_filename     = "memory.json"     ;
  const char *log_filename              = "log.txt"         ;
  const char *subdir_name               = "jeff-qt"         ;

  // Objects:
  QString _settings_path;

  // Functions described in `json.cpp`:
  void check_or_create_subdir();
  QJsonArray read_json(QFile *file);
  void write_json(QFile *savefile, const QJsonArray &json_array);
};

#endif
