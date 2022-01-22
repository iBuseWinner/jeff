#include "json.h"

Json::Json(QString settingsPath, QObject *parent) : QObject(parent), _settings_path(settingsPath) {}

/*!
 * @fn Json::read_source_list
 * @brief Reads the store and loads a list of connected sources.
 * @returns list of sources
 * @sa Sources
 */
Sources Json::read_source_list(SQLite *sql) {
  auto *store = new QFile(_settings_path + QDir::separator() + sources_store_filename, this);
  QJsonArray sources_json = read_json(store);
  Sources sources;
  for (auto source_json : sources_json)
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
 * @sa Messages
 */
Messages Json::read_message_history(QFile *file) {
  QJsonArray messages_json = read_json(file);
  Messages message_history;
  for (auto obj : messages_json) message_history.append(MessageData(obj.toObject()));
  return message_history;
}

/*!
 * @fn Json::read_NLP_cache
 * @brief Reads the expressions most commonly used in Jeff's answers.
 * @returns cache for NLPmodule
 * @sa Cache
 */
Cache Json::read_NLP_cache() {
  auto *store = new QFile(_settings_path + QDir::separator() + cache_store_filename, this);
  if (not store->exists()) return Cache();
  QJsonArray cache_json = read_json(store);
  Cache cache;
  for (auto obj : cache_json) cache.append(new Expression(obj.toObject()));
  return cache;
}

/*!
 * @fn Json::read_scripts
 * @brief Reads scripts metadata that is intended to enhance Jeff's capabilities.
 * @returns scripts metadata
 * @sa Scripts
 */
Scripts Json::read_scripts() {
  auto *store = new QFile(_settings_path + QDir::separator() + scripts_store_filename);
  if (not store->exists()) return Scripts();
  QJsonArray scripts_json = read_json(store);
  Scripts scripts;
  for (auto obj : scripts_json) scripts.append(ScriptMetadata(obj.toObject()));
  return scripts;
}

/*!
 * @fn Json::write_source_list
 * @brief Writes @a sourceList to @a savefile.
 * @param[in] source_list list of sources' properties
 */
void Json::write_source_list(SQLite *sql, Sources sources) {
  QJsonArray cs;
  for (int i = 0; i < sources.length(); i++) {
    cs.append(Source::to_json(sources[i]));
    // Some properties of sources are stored directly in the database itself in "tables".
    // Jeff writes them there.
    sql->write_source(sources[i]);
  }
  auto *savefile = new QFile(_settings_path + QDir::separator() + sources_store_filename, this);
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
  for (auto message : message_history) message_history_json.append(message.to_json());
  write_json(file, message_history_json);
}

/*!
 * @fn Json::write_NLP_cache
 * @brief Saves @a cache to @a file.
 * @param[in] cache list of expressions
 */
void Json::write_NLP_cache(Cache cache) {
  QJsonArray cache_json;
  for (auto expression : cache) cache_json.append(expression->to_json());
  auto *file = new QFile(_settings_path + QDir::separator() + cache_store_filename, this);
  write_json(file, cache_json);
}

/*!
 * @fn Json::write_scripts()
 * @brief Saves @a scripts to @a file.
 * @param[in] scripts metadata to save
 */
void Json::write_scripts(Scripts scripts) {
  QJsonArray scripts_json;
  for (auto script : scripts) scripts_json.append(script.to_json());
  auto *file = new QFile(_settings_path + QDir::separator() + scripts_store_filename, this);
  write_json(file, scripts_json);
}

/*!
 * @fn Json::to_json
 * @brief Translates a dictionary to JSON.
 * @param[in] keystore with keys and values
 * @returns string with JSON
 */
std::string Json::to_json(KeyStore keystore) {
  QJsonArray arr;
  for (auto key : keystore.keys()) {
    QJsonArray a1;
    a1.append(key);
    a1.append(keystore[key]);
    arr.append(a1);
  }
  QJsonDocument doc(arr);
  return doc.toJson(QJsonDocument::Compact).toStdString();
}

/*!
 * @fn Json::from_json
 * @brief Translates a JSON string into a data dictionary.
 * @param[in] str to translate
 * @returns translated keystore
 */
KeyStore Json::from_json(std::string str) {
  auto *errors = new QJsonParseError();
  auto doc = QJsonDocument::fromJson(QString(str.c_str()).toUtf8(), errors);
  if (errors->error != QJsonParseError::NoError) {
    emit json_error(errors->errorString());
    delete errors;
    return KeyStore();
  }
  KeyStore keystore;
  for (auto val : doc.array()) {
    auto va = val.toArray();
    if (va.size() != 2) {
      emit json_error(tr("The number of elements in the object is not equal to two."));
      return keystore;
    }
    keystore[va[0].toString()] = va[1];
  }
  return keystore;
}

/*!
 * @fn Json::read_json
 * @brief Universal JSON read function.
 * @details Additionally checks the file for errors.
 * @param[in,out] file file to read
 * @returns QJsonArray read from file
 */
QJsonArray Json::read_json(QFile *file) {
  if (not file->exists()) return QJsonArray();
  if (not file->open(QIODevice::ReadOnly | QIODevice::Text)) return QJsonArray();
  QTextStream textStream(file);
  auto *errors = new QJsonParseError();
  QJsonDocument document = QJsonDocument::fromJson(textStream.readAll().toUtf8(), errors);
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
  if (not savefile->open(QIODevice::WriteOnly | QIODevice::Text)) return;
  QJsonDocument jsonDocument(json_array);
  QTextStream textStream(savefile);
  textStream << jsonDocument.toJson(QJsonDocument::Compact);
  savefile->close();
}
