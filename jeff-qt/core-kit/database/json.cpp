#include "json.hpp"

/*! @brief The constructor. */
Json::Json(QString settingsPath, QObject *parent) : QObject(parent), _settings_path(settingsPath) {
  check_or_create_subdir();
  _settings_path = _settings_path + QDir::separator() + subdir_name;
  Yellog::EnableFileOutput(QString("%1%2%3").arg(_settings_path).arg(QDir::separator()).arg(log_filename).toStdString().c_str());
}

/*! @brief Reads the store and loads a list of connected sources. */
Sources Json::read_source_list(SQLite *sql) {
  QFile store = QFile(_settings_path + QDir::separator() + sources_store_filename);
  if (not store.exists()) return Sources();
  QJsonArray sources_json = read_json(&store);
  Sources sources;
  for (auto source_json : sources_json)
    /*! Some properties of sources are stored directly in the database itself
     *  in tables. Json reads them too (@sa SQLite::load).  */
    sources.append(sql->load_source(Source(source_json.toObject())));
  return sources;
}

/*! @brief Recreates message history from @a file. */
MessagesMeta Json::read_message_history(QFile *file) {
  QJsonArray messages_json = read_json(file);
  MessagesMeta message_history;
  for (auto obj : messages_json) message_history.append(MessageMeta(obj.toObject()));
  return message_history;
}

/*! @brief Restores @a message_history from default storage. */
MessagesMeta Json::read_message_history() {
  QFile store = QFile(_settings_path + QDir::separator() + history_store_filename);
  return read_message_history(&store);
}

/*! @brief Reads the expressions most commonly used in Jeff's answers - @a cache. */
Cache Json::read_NLP_cache() {
  QFile store = QFile(_settings_path + QDir::separator() + cache_store_filename);
  if (not store.exists()) return Cache();
  QJsonArray cache_json = read_json(&store);
  Cache cache;
  for (auto obj : cache_json) cache.append(Expression(obj.toObject()));
  return cache;
}

/*! @brief Reads an extensions' metadata. */
ExtensionsMeta Json::read_extensions() {
  QFile store = QFile(_settings_path + QDir::separator() + extensions_store_filename);
  if (not store.exists()) return ExtensionsMeta();
  QJsonArray extensions_json = read_json(&store);
  ExtensionsMeta extensions_meta;
  for (auto extension_props : extensions_json) {
    if (not extension_props.isObject()) continue;
    auto ext_props_obj = extension_props.toObject();
    if (not ext_props_obj.contains("origin") and not ext_props_obj.contains("enabled")) continue;
    auto *extension_meta = ExtensionMeta::from_origin(ext_props_obj["origin"].toString(), ext_props_obj["enabled"].toBool());
    if (not extension_meta) continue;
    extensions_meta.append(extension_meta);
  }
  return extensions_meta;
}

/*! @brief Reads memory - @a keystore. */
KeyStore Json::read_memory() {
  QFile store = QFile(_settings_path + QDir::separator() + memory_store_filename);
  if (not store.exists()) return KeyStore();
  QJsonArray memory_json = read_json(&store);
  KeyStore keystore;
  for (auto cell : memory_json) {
    auto cell_obj = cell.toObject();
    keystore[cell_obj.keys()[0]] = cell_obj[cell_obj.keys()[0]];
  }
  return keystore;
}

/*! @brief Saves @a sources. */
void Json::write_source_list(SQLite *sql, Sources sources) {
  QJsonArray sources_json;
  for (int i = 0; i < sources.length(); i++) {
    sources_json.append(sources[i].to_json());
    // Some properties of sources are stored directly in the database itself in "tables".
    // Jeff writes them there.
    sql->write_source(sources[i]);
  }
  QFile file = QFile(_settings_path + QDir::separator() + sources_store_filename);
  write_json(&file, sources_json);
}

/*! @brief Saves @a message_history. */
void Json::write_message_history(MessagesMeta message_history, QFile *file) {
  QJsonArray message_history_json;
  for (auto message : message_history) message_history_json.append(message.to_json());
  write_json(file, message_history_json);
}

/*! @brief Saves @a message_history at the default storage. */
void Json::write_message_history(MessagesMeta message_history) {
  QFile store = QFile(_settings_path + QDir::separator() + history_store_filename);
  write_message_history(message_history, &store);
}

/*! @brief Saves NLPmodule's @a cache. */
void Json::write_NLP_cache(Cache cache) {
  QJsonArray cache_json;
  for (auto expression : cache) cache_json.append(expression.to_json());
  QFile file = QFile(_settings_path + QDir::separator() + cache_store_filename);
  write_json(&file, cache_json);
}

/*! @brief Saves extensions' metadata. */
void Json::write_extensions(ExtensionsMeta extensions_meta) {
  QJsonArray json_arr;
  for (auto *extension_meta : extensions_meta) json_arr.append(QJsonObject({
    {"origin", extension_meta->origin},
    {"enabled", extension_meta->enabled}
  }));
  QFile file = QFile(_settings_path + QDir::separator() + extensions_store_filename);
  write_json(&file, json_arr);
}

/*! @brief Saves @a memory. */
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

/*! @brief Universal JSON read from @a file function. */
QJsonArray Json::read_json(QFile *file) {
  if (not file->exists()) return QJsonArray();
  if (not file->open(QIODevice::ReadOnly | QIODevice::Text)) return QJsonArray();
  QTextStream textStream(file);
  QJsonParseError errors;
  QJsonDocument document = QJsonDocument::fromJson(textStream.readAll().toUtf8(), &errors);
  if (errors.error != QJsonParseError::NoError) {
    Yellog::Error("At Json::read_json:");
    Yellog::Error("\tError's parsing code: %d", int(errors.error));
    return QJsonArray();
  }
  file->close();
  return document.array();
}

/*! @brief Checks @a savefile for access and writes @a json_array. */
void Json::write_json(QFile *savefile, const QJsonArray &json_array) {
  if (not savefile->open(QIODevice::WriteOnly | QIODevice::Text)) {
    Yellog::Error("At Json::write_json:");
    Yellog::Error("\tCannot open savefile \"%s\" as rw/text.", savefile->fileName().toStdString().c_str());
    return;
  }
  QJsonDocument doc(json_array);
  QTextStream textStream(savefile);
  textStream << doc.toJson(QJsonDocument::Compact);
  savefile->close();
}

/*! @brief Checks */
void Json::check_or_create_subdir() {
  QDir dir;
  auto subdir_path = _settings_path + QDir::separator() + subdir_name;
  if (not dir.exists(subdir_path)) dir.mkdir(subdir_path);
}
