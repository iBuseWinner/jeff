#include "json.h"

/*! @brief The constructor. */
Json::Json(QString settingsPath, QObject *parent) : QObject(parent), _settings_path(settingsPath) {}

/*! @brief Reads the store and loads a list of connected sources. */
Sources Json::read_source_list(SQLite *sql) {
  QFile store = QFile(_settings_path + QDir::separator() + sources_store_filename, this);
  if (not store.exists()) return Sources();
  QJsonArray sources_json = read_json(&store);
  Sources sources;
  for (auto source_json : sources_json)
    /*! Some properties of sources are stored directly in the database itself
     *  in tables. Json reads them too (@sa SQLite::load).  */
    sources.append(sql->load_source(Source(source_json.toObject())));
  return sources;
}

/*! @brief Recreates message history from file. */
Messages Json::read_message_history(QFile *file) {
  QJsonArray messages_json = read_json(file);
  Messages message_history;
  for (auto obj : messages_json) message_history.append(MessageData(obj.toObject()));
  return message_history;
}

/*! @brief Reads the expressions most commonly used in Jeff's answers. */
Cache Json::read_NLP_cache() {
  QFile store = QFile(_settings_path + QDir::separator() + cache_store_filename, this);
  if (not store.exists()) return Cache();
  QJsonArray cache_json = read_json(&store);
  Cache cache;
  for (auto obj : cache_json) cache.append(Expression(obj.toObject()));
  return cache;
}

/*! @brief Reads scripts' metadata. */
Scripts Json::read_scripts() {
  QFile store = QFile(_settings_path + QDir::separator() + scripts_store_filename);
  if (not store.exists()) return Scripts();
  QJsonArray scripts_json = read_json(&store);
  Scripts scripts;
  for (auto obj : scripts_json) scripts.append(ScriptMetadata(obj.toObject()));
  return scripts;
}

/*! @brief Reads memory. */
KeyStore Json::read_memory() {
  QFile store = QFile(_settings_path + QDir::separator() + memory_store_filename);
  if (not store.exists()) return KeyStore();
  QJsonArray memory_json = read_json(&store);
  KeyStore keystore;
  for (auto cell : memory_json) keystore[cell.keys()[0]] = memory_json[cell[cell.keys()[0]]];
  return keystore;
}

/*! @brief Saves list of sources. */
void Json::write_source_list(SQLite *sql, Sources sources) {
  QJsonArray sources_json;
  for (int i = 0; i < sources.length(); i++) {
    sources_json.append(Source::to_json(sources[i]));
    // Some properties of sources are stored directly in the database itself in "tables".
    // Jeff writes them there.
    sql->write_source(sources[i]);
  }
  QFile file = QFile(_settings_path + QDir::separator() + sources_store_filename, this);
  write_json(&file, sources_json);
}

/*! @brief Saves message history. */
void Json::write_message_history(Messages message_history, QFile *file) {
  QJsonArray message_history_json;
  for (auto message : message_history) message_history_json.append(message.to_json());
  write_json(file, message_history_json);
}

/*! @brief Saves NLPmodule's cache. */
void Json::write_NLP_cache(Cache cache) {
  QJsonArray cache_json;
  for (auto expression : cache) cache_json.append(expression.to_json());
  QFile file = QFile(_settings_path + QDir::separator() + cache_store_filename);
  write_json(&file, cache_json);
}

/*! @brief Saves scripts' metadata. */
void Json::write_scripts(Scripts scripts) {
  QJsonArray scripts_json;
  for (auto script : scripts) scripts_json.append(script.to_json());
  QFile file = QFile(_settings_path + QDir::separator() + scripts_store_filename);
  write_json(&file, scripts_json);
}

/*! @brief Saves memory. */
void Json::write_memory(KeyStore memory) {
  QJsonArray memory_json;
  for (auto cell_key : memory.keys()) {
    QJsonObject cell_obj;
    cell_obj[cell_key] = memory[cell_key];
    memory_json.append(cell_obj);
  }
  QFile file = QFile(_settings_path + QDir::separator() + memory_store_filename);
  write_json(&file, memory_json);
}

/*! @brief Universal JSON read function. */
QJsonArray Json::read_json(QFile *file) {
  if (not file->exists()) return QJsonArray();
  if (not file->open(QIODevice::ReadOnly | QIODevice::Text)) return QJsonArray();
  QTextStream textStream(file);
  QJsonParseError errors;
  QJsonDocument document = QJsonDocument::fromJson(textStream.readAll().toUtf8(), &errors);
  if (errors.error != QJsonParseError::NoError) return QJsonArray();
  file->close();
  return document.array();
}

/*! @brief Checks the file for access and writes file. */
void Json::write_json(QFile *savefile, QJsonArray json_array) {
  if (not savefile->open(QIODevice::WriteOnly | QIODevice::Text)) return;
  QJsonDocument doc(json_array);
  QTextStream textStream(savefile);
  textStream << doc.toJson(QJsonDocument::Compact);
  savefile->close();
}
