#include "basis.hpp"

/*! @brief The constructor. */
Basis::Basis(QObject *parent) : QObject(parent) {
  json = new Json(get_settings_path(), this); /*!< @details Json object will be created first as it inits yelloger. */
  sql = new SQLite(this);
  cacher = new Cacher(this);
  load_sources();
  load_memory();
  // Setting the `jeff-lang` value:
  {
    auto l = QLocale::system().name();
    l.truncate(2);
    memory("jeff-lang", l);
  }
  // Setting the `jeff-bundle-dir` value:
  {
    auto p = get_settings_path() + "/" + Json::subdir_name + "/" + Basis::bundle_dir_name;
    QDir dir;
    if (not dir.exists(p)) dir.mkdir(p);
    memory("jeff-bundle-dir", p);
  }
}

/*! @brief The destructor. */
Basis::~Basis() {
  save_memory();
  _settings.sync();
  if (custom_scanner) delete custom_scanner;
  if (custom_composer) delete custom_composer;
}

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
  Yellog::Trace("Checking default source...");
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

/*! @brief Gets the default source used for autoadding expressions etc.
 *  Automatically set by Jeff when selected the first source.  */
Source Basis::get_default_source() {
  Yellog::Trace("Getting default source...");
  QVariant path = read(defaultSourcePath);
  QVariant container = read(defaultSourceContainer);
  if (not path.isValid() or not container.isValid()) set_first_source_as_default();
  QString p = path.toString();
  QString c = container.toString();
  Source s;
  s.path = p;
  s.table_name = c;
  if (not sources().contains(s)) set_first_source_as_default();
  return s;
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
  Yellog::Trace("Loading sources...");
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
    Yellog::Trace("We need to send some memory values to script");
    QJsonArray val_keys = object[readMemoryWk].toArray();
    if (not val_keys.isEmpty()) {
      QJsonObject obj;
      for (auto key : val_keys) {
        if (not key.isString()) continue;
        QString k = key.toString();
        auto val = memory(k);
        if (val.isNull()) continue;
        obj[k] = memory(k);
      }
      transport[memoryValuesWk] = obj;
    }
  }
  return transport;
}

/*! @brief Handles @an object that came from script or extension. */
void Basis::handle_from_script(const QJsonObject &object, bool except_send) {
  /*! @details Common logic part. */
  if (object.contains(writeMemoryWk)) {
    if (object[writeMemoryWk].isObject()) {
      Yellog::Trace("We need to write some memory values from script");
      auto to_store = object[writeMemoryWk].toObject();
      for (auto key : to_store.keys()) {
        auto value = to_store[key];
        memory(key, value);
      }
    }
  }
  if (object.contains(addExpressionWk)) {
    if (object[addExpressionWk].isArray()) {
      Yellog::Trace("We need to add the expression in the default source");
      auto to_add = object[addExpressionWk].toArray();
      auto default_source = get_default_source();
      for (auto expr_json_v : to_add) {
        if (not expr_json_v.isObject()) continue;
        auto expr_json = expr_json_v.toObject();
        if (
          not expr_json.contains("activator_text") or
          not expr_json.contains("reagent_text") or
          not expr_json.contains("properties") or
          not expr_json.contains("exec")
        ) continue;
        Expression new_expr;
        new_expr.activator_text = expr_json["activator_text"]                 .toString();
        new_expr.reagent_text   = expr_json["reagent_text"]                   .toString();
        new_expr.properties     = Phrase::parse_props(expr_json["properties"] .toObject());
        new_expr.exec           = expr_json["exec"]                           .toBool();
        sql->insert_expression(default_source, new_expr);
      }
    }
  }
  /*! @details Scenario logic part. */
  bool send_auth = false;
  if (
    object.contains(scenarioReadyWk) and
    object.contains(scenarioAddrWk)  and
    object.contains(scenarioPortWk)  and 
    object.contains(scenarioNameWk)  and
    not except_send
  ) {
    if (
      object[scenarioReadyWk].toBool() and
      not object[scenarioNameWk].toString().isEmpty()
    ) {
      Yellog::Trace("Got a scenario");
      auto server_addr = QHostAddress(object[scenarioAddrWk].toString());
      if (server_addr.isNull()) server_addr = QHostAddress("127.0.0.1");
      auto server_port = quint16(object[scenarioPortWk].toInt());
      auto name = object[scenarioNameWk].toString();
      auto scenario_token = sql->generate_uuid();
      ScenarioServerMeta scenario_meta;
      scenario_meta.server_addr = server_addr;
      scenario_meta.server_port = server_port;
      scenario_meta.name = name;
      scenario_meta.auth_key = scenario_token;
      if (_scenario_token.isEmpty()) {
        _scenario_token = scenario_token;
        emit start_scenario(scenario_meta);
        send_auth = true;
      } else {
        emit schedule_scenario(scenario_meta);
      }
    }
  } else if (object.contains(scenarioContinueWk) and object.contains(scenarioTokenWk) and not except_send) {
    if (
      object[scenarioContinueWk].toBool() and
      object[scenarioTokenWk].toString() == _scenario_token
    ) {
      send_auth = true;
    }
  } else if (object.contains(scenarioFinishWk) and object.contains(scenarioTokenWk) and not except_send) {
    if (object[scenarioFinishWk].toBool() and object[scenarioTokenWk].toString() == _scenario_token) {
      clear_stoken();
      emit shutdown_scenario();
      send_auth = true;
    }
  } else { send_auth = _scenario_token.isEmpty(); }
  /*! @details Daemons and servers logic part. */
  if (not except_send and send_auth) {
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
    } else if (object.contains(sendInfoWk)) {
      QString info_message = object[sendInfoWk].toString();
      emit info(info_message);
    }
  }
}

/*! @brief Sets the selected custom scanner. */
void Basis::set_custom_scanner(ScriptMeta *_custom_scanner) {
  custom_scanner = _custom_scanner;
  emit custom_scanner_changed();
}

/*! @brief Sets the selected custom composer. */
void Basis::set_custom_composer(ScriptMeta *_custom_composer) {
  custom_composer = _custom_composer;
  emit custom_composer_changed();
}
