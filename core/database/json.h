#ifndef JSON_H
#define JSON_H

#include "core/database/sqlite.h"
#include "core/model/message.h"
#include "core/model/source.h"
#include "core/model/nlp/cache.h"
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QList>

/*!
 * @class Json
 * @brief Provides methods for reading and writing data in/from json files.
 * @sa SQLite
 */
class Json : public QObject {
  Q_OBJECT
public:
  /*!
   * @fn Json::Json
   * @brief The constructor.
   */
  Json(QObject *parent = nullptr);

  // Functions described in `json.cpp`:
  Sources read_source_list(SQLite *sql, QString settingsPath);
  Messages read_message_history(QFile *file);
  Cache read_NLP_cache(QString settingsPath);
  void write_source_list(SQLite *sql, QString settingsPath,
                         Sources source_list);
  void write_message_history(Messages message_history, QFile *file);
  void write_NLP_cache(Cache cache, QString settingsPath);

signals:
  /*!
   * @brief Reports an error while parsing JSON.
   */
  QString json_error(QString error_text);

private:
  // Constants:
  inline static const QString sources_store_filename = "jeff_sources.json";
  inline static const QString cache_store_filename = "nlp_module_cache.json";

  // Functions described in `json.cpp`:
  QJsonArray read_json(QFile *file);
  void write_json(QFile *savefile, QJsonArray json_array);
};

#endif // JSON_H
