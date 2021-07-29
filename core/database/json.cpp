#include "json.h"

Json::Json(QObject *parent) : QObject(parent) {}

/*!
 * @fn Json::read_source_list
 * @brief Reads the store and loads a list of connected sources.
 * @returns list of sources
 */
Sources Json::read_source_list(SQLite *sql, QString settingsPath) {
  auto *store = new QFile(
      settingsPath + QDir::separator() + sources_store_filename, this);
  QJsonArray sources_json = read_json(store);
  Sources sources;
  for (const QJsonValue &source_json : qAsConst(sources_json))
    /*!
     * Some properties of sources are stored directly in the database itself
     * in tables. Json reads them too.
     * @sa SQLite::load
     */
    sources.append(sql->load_source(Source(source_json.toObject())));
  return sources;
}

/*!
 * @fn Json::read_message_history
 * @brief Recreates message history from file.
 * @param[in,out] file QFile to read message history from
 * @returns list of messages from file
 */
Messages Json::read_message_history(QFile *file) {
  QJsonArray messages_json = read_json(file);
  Messages message_history;
  for (const QJsonValue &obj : qAsConst(messages_json))
    message_history.append(Message(obj.toObject()));
  return message_history;
}

Cache Json::read_NLP_cache(QString settingsPath) {
  auto *store =
      new QFile(settingsPath + QDir::separator() + cache_store_filename, this);
  if (not store->exists()) {
    delete store;
    return Cache();
  }
  QJsonArray cache_json = read_json(store);
  Cache cache;
  for (const QJsonValue &obj : qAsConst(cache_json))
    cache.append(new Expression(obj.toObject()));
  delete store;
  return cache;
}

/*!
 * @fn Json::write_source_list
 * @brief Writes @a sourceList to @a savefile.
 * @param[in] source_list list of sources' properties
 */
void Json::write_source_list(SQLite *sql, QString settingsPath,
                             Sources source_list) {
  QJsonArray cs;
  for (auto source : source_list) {
    cs.append(source.to_json());
    // Some properties of sources are stored directly in the database itself
    // in "tables". ASW writes them there.
    sql->write_source(source);
  }
  auto *savefile =
      new QFile(settingsPath + QDir::separator() + sources_store_filename);
  write_json(savefile, cs);
}

/*!
 * @fn Json::write_message_history
 * @brief Saves @a message_history to @a file.
 * @param[in] message_history list of messages
 * @param[in,out] file file to save there
 */
void Json::write_message_history(Messages message_history, QFile *file) {
  QJsonArray message_history_json;
  for (auto message : message_history)
    message_history_json.append(message.to_json());
  write_json(file, message_history_json);
}

/*!
 * @fn Json::write_cache
 * @brief Saves @a cache to @a file.
 * @param[in] cache list of expressions
 */
void Json::write_NLP_cache(Cache cache, QString settingsPath) {
  QJsonArray cache_json;
  for (auto expression : cache)
    cache_json.append(expression->to_json());
  auto *file = new QFile(settingsPath + QDir::separator() + cache_store_filename, this);
  write_json(file, cache_json);
  delete file;
}

/*!
 * @fn Json::read_json
 * @brief Universal JSON read function.
 * @details Additionally checks the file for errors.
 * @param[in,out] file file to read
 * @returns QJsonArray read from file
 */
QJsonArray Json::read_json(QFile *file) {
  if (not file->open(QIODevice::ReadOnly | QIODevice::Text))
    return QJsonArray();
  QTextStream textStream(file);
  auto *errors = new QJsonParseError();
  QJsonDocument document =
      QJsonDocument::fromJson(textStream.readAll().toUtf8(), errors);
  if (errors->error != QJsonParseError::NoError) {
    emit json_error(errors->errorString());
    delete errors;
    return QJsonArray();
  }
  delete errors;
  file->close();
  return document.array();
}

/*!
 * @fn Json::write_json
 * @brief Writes @a jsonArray to @a savefile.
 * @details Additionally checks the file for access.
 * @param[in,out] savefile file to save there
 * @param[in] json_array array to write in @a savefile
 */
void Json::write_json(QFile *savefile, QJsonArray json_array) {
  if (not savefile->open(QIODevice::WriteOnly | QIODevice::Text))
    return;
  QJsonDocument jsonDocument(json_array);
  QTextStream textStream(savefile);
  textStream << jsonDocument.toJson(QJsonDocument::Indented);
  savefile->close();
}
