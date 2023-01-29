#include "basis.hpp"

/*! @brief Checks the settings file for any errors. */
void Basis::check_settings_file() {
  if (not accessible()) {
    emit warn(tr("An access error occurred (e.g. trying to write to a read-only file)."));
    Yellog::Warn("Settings file' access error occurred.");
  } else if (not correct()) {
    emit warn(tr("A format error occurred (e.g. loading a malformed file)."));
    Yellog::Warn("Settings file' format error occurred.");
  }
}

/*! @brief Sends a warning to the screen. */
void Basis::warn_about(QString warning_text) {
  Yellog::Warn(warning_text.toStdString().c_str());
  emit warn(warning_text);
}

/*! @brief Checks if there is a standard source, and otherwise sets the first source in the list as standard.  */
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
  if (not _sources.isEmpty()) {
    write(defaultSourcePath, _sources[0].path);
    write(defaultSourceContainer, _sources[0].table_name);
    Yellog::Info("New default source: %s, %s",
                 _sources[0].path.toStdString().c_str(),
                 _sources[0].table_name.toStdString().c_str());
  } else {
    write(defaultSourcePath, "");
    write(defaultSourceContainer, "");
    Yellog::Info("New empty default source. No choice to save smth.");
  }
}

/*! @brief Sets the value of the parameter. */
void Basis::write(const QString &key, const QVariant &data) {
  if (not correct()) {
    /*! If the file is incorrectly formatted, Basis just clears the file. */
    Yellog::Warn("Wow!.. Settings file isn't correct. Clearing...");
    _settings.clear();
    _settings.sync();
  }
  _settings.setValue(key, data);
}

/*! @brief Loads @a _sources from file. */
void Basis::load_sources() {
  if (not _sources.isEmpty()) _sources.clear();
  Sources tmp = json->read_source_list(sql);
  for (int i = 0; i < tmp.length(); i++) {
    if (db_exists(tmp[i].path))
      if (not _sources.contains(tmp[i]))
        _sources.append(tmp[i]);
  }
  Yellog::Trace("Sources loaded from disk.");
  emit sources_changed();
}

/*! @brief @returns @a _sources. */
Sources Basis::sources() { return _sources; }

/*! @brief Saves and updates @a sources list. */
void Basis::sources(Sources s) {
  _sources = s;
  json->write_source_list(sql, _sources);
  Yellog::Trace("Sources saved to disk.");
  check_default_source();
  emit sources_changed();
}

/*! @brief Loads @a _memory from file. */
void Basis::load_memory() { _memory = json->read_memory(); }
/*! @brief Saves @a _memory to file. */
void Basis::save_memory() { json->write_memory(_memory); }
/*! @brief Writes @a data to memory by @a key. */
void Basis::memory(const QString &key, QJsonValue data) { _memory[key] = data; }
/*! @brief Reads @a data from memory by @a key. */
QJsonValue Basis::memory(const QString &key) { return _memory[key]; }

/*! @brief Prepares @a transport to send into script. */
QJsonObject Basis::handle_to_script(const QJsonObject &object) {
  QJsonObject transport;
  if (object.contains(readMemoryWk)) {
    QJsonArray val_keys = object[readMemoryWk].toArray();
    if (not val_keys.isEmpty()) {
      QJsonObject obj;
      for (auto key : val_keys) {
        if (not key.isString()) continue;
        QString k = key.toString();
        obj[k] = memory(k);
      }
      transport[memoryValuesWk] = obj;
    }
  }
  return transport;
}

/*! @brief Handles @an object that came from script. */
void Basis::handle_from_script(const QJsonObject &object, bool except_send) {
  bool send_auth = false;
  if (
    object.contains(scenarioReadyWk) and
    object.contains(scenarioAddrWk) and
    object.contains(scenarioPortWk)
  ) {
    if (object[scenarioReadyWk].toBool()) {
      auto server_addr = QHostAddress(object[scenarioAddrWk].toString());
      if (server_addr.isNull()) server_addr = QHostAddress("127.0.0.1");
      auto server_port = quint16(object[scenarioPortWk].toInt());
      ScenarioServerMeta scenario_meta;
      scenario_meta.server_addr = server_addr;
      scenario_meta.server_port = server_port;
      _scenario_token = sql->generate_uuid();
      scenario_meta.auth_key = _scenario_token;
      emit start_scenario(scenario_meta);
      send_auth = true;
    }
  } else if (object.contains(scenarioContinueWk) and object.contains(scenarioTokenWk)) {
    if (
      object[scenarioContinueWk].toBool() and
      object[scenarioTokenWk].toString() == _scenario_token
    ) {
      send_auth = true;
    }
  } else if (object.contains(scenarioFinishWk) and object.contains(scenarioTokenWk)) {
    if (object[scenarioFinishWk].toBool() and object[scenarioTokenWk].toString() == _scenario_token) {
      clear_stoken();
      emit shutdown_scenario();
      send_auth = true;
    }
  } else { send_auth = _scenario_token.isEmpty(); }
  if (object.contains(writeMemoryWk)) {
    QJsonArray to_store = object[writeMemoryWk].toArray();
    if (not to_store.isEmpty()) {
      for (auto pair : to_store) {
        if (not pair.isObject()) continue;
        auto pair_obj = pair.toObject();
        if (not pair_obj.contains("key") or not pair_obj.contains("value")) continue;
        if (not pair_obj["key"].isString()) continue;
        memory(pair_obj["key"].toString(), pair_obj["value"]);
      }
    }
  }
  if (not except_send and send_auth) {
    /*! This block have used to send messages by daemons and servers. */
    if (object.contains(sendWk)) {
      QString message = object[sendWk].toString();
      emit send(message);
    } else if (object.contains(searchAgainWk)) {
      QString rephrased_message = object[searchAgainWk].toString();
      emit search_again(rephrased_message);
    } else if (object.contains(sendAsUserWk)) {
      QString outter_message = object[sendAsUserWk].toString();
      emit send_as_user(outter_message);
    } else if (object.contains(sendStatusWk)) {
      QJsonObject status_obj = object[sendStatusWk].toObject();
      if (not status_obj.isEmpty()) {
        QPair<QString, QString> id_and_message(status_obj["id"].toString(),
                                               status_obj["msg"].toString());
        emit send_status(id_and_message);
      }
    } else if (object.contains(sendWarningWk)) {
      QString warn_message = object[sendWarningWk].toString();
      emit warn(warn_message);
    }
  }
}
