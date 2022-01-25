#include "basis.h"

/*! @brief Checks the settings file for any errors. */
void Basis::check_settings_file() {
  if (not accessible())
    emit settings_warning(
      tr("An access error occurred (e.g. trying to write to a read-only file).")
    );
  else if (not correct())
    emit settings_warning(tr("A format error occurred (e.g. loading a malformed file)."));
}

/*! @brief Checks if there is a standard source, and otherwise sets the first source in the list 
 * as standard. */
void Basis::check_default_source() {
  QVariant path = read(defaultSourcePath);
  QVariant container = read(defaultSourceContainer);
  if (not path.isValid() or not container.isValid()) set_first_source_as_default();
  QString p = path.toString();
  QString c = container.toString();
  Source s;
  s.path = p;
  s.table_name = c;
  if (not sources().contains(s)) set_first_source_as_default();
}

/*! @brief @see Basis::check_default_source */
void Basis::set_first_source_as_default() {
  sources_mutex.lock();
  if (not sources.isEmpty()) {
    write(defaultSourcePath, sources[0].path);
    write(defaultSourceContainer, sources[0].table_name);
  } else {
    write(defaultSourcePath, "");
    write(defaultSourceContainer, "");
  }
  sources_mutex.unlock();
}

/*! @brief Sets the value of the parameter. */
void Basis::write(const QString &key, const QVariant &data) {
  if (not correct()) {
    /*! If the file is incorrectly formatted, Basis will not be able to restore the data structure,
     *  so it clears the file.  */
    _settings.clear();
    _settings.sync();
  }
  _settings.setValue(key, data);
}

/*! @brief Loads @a _sources from file. */
void Basis::load_sources() {
  sources_mutex.lock();
  if (not _sources.isEmpty()) _sources.clear();
  Sources tmp = json->read_source_list(sql);
  for (int i = 0; i < tmp.length(); i++) {
    if (db_exists(tmp[i].path))
      if (not _sources.contains(tmp[i]))
        _sources.append(tmp[i]);
  }
  sources_mutex.unlock();
}

/*! @brief @returns @a _sources. */
Sources Basis::sources() {
  sources_mutex.lock();
  auto s = _sources;
  sources_mutex.unlock();
  return s;
}

/*! @brief Saves and updates @a sources list. */
void Basis::sources(Sources s) {
  sources_mutex.lock();
  _sources = s;
  sources_mutex.unlock();
  json->write_source_list(sql, s);
  check_default_source();
}

/*! * @brief Writes @a value to context by @a key. */
void context(const QString &key, const QString &value) {
  context_mutex.lock();
  _context[key] = value;
  context_mutex.unlock();
}

/*! @brief Reads @a value from context by @a key. */
QString context(const QString &key) {
  context_mutex.lock();
  auto value = _context[key];
  context_mutex.unlock();
  return value;
}

/*! @brief Loads @a _memory from file. */
void Basis::load_memory() {
  KeyStore m = json->read_memory();
  memory_mutex.lock();
  _memory = m;
  memory_mutex.unlock();
}

/*! @brief Saves @a _memory to file. */
void Basis::save_memory() {
  memory_mutex.lock();
  KeyStore m = _memory;
  memory_mutex.unlock();
  json->write_memory(m);
}

/*! * @brief Writes @a data to memory by @a key. */
void Basis::memory(const QString &key, QJsonValue data) {
  memory_mutex.lock();
  _memory[key] = data;
  memory_mutex.unlock();
}

/*! @brief Reads @a data from memory by @a key. */
QJsonValue Basis::memory(const QString &key) {
  memory_mutex.lock();
  auto data = _memory[key];
  memory_mutex.unlock();
  return data;
}

/*! @brief Prepares @a transport to send into script. */
QJsonObject handle_to_script(const QJsonObject &object) {
  QJsonObject transport;
  if (object.contains(need_values)) {
    QJsonArray val_keys = object[need_values].toArray();
    if (not val_keys.isEmpty()) {
      transport[basis->values] = QJsonObject();
      for (auto key : val_keys) {
        if (not key.isString()) continue;
        QString k = key.toString();
        transport[basis->values].toObject()[k] = memory(k);
      }
    }
  }
  return transport;
}

/*! @brief Handles @a object that came from script. */
void handle_from_script(const QJsonObject &object, bool except_send = false) {
  if (object.contains(basis->store_values)) {
    QJsonArray to_store = object[basis->store_values].toArray();
    if (not to_store.isEmpty()) {
      for (auto pair : to_store) {
        if (pair.isObject()) continue;
        auto pair_obj = pair.toObject();
        if (not pair_obj.contains("key") or not pair_obj.contains("value")) continue;
        if (not pair_obj["key"].isString()) continue;
        memory(pair_obj["key"].toString(), pair_obj["value"]);
      }
    }
  }
}
