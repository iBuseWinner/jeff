#include "basis.h"

/*!
 * @fn Basis::check_settings_file
 * @brief Checks the settings file for any errors.
 */
void Basis::check_settings_file() {
  if (not accessible())
    emit settings_warning(
        tr("An access error occurred (e.g. trying to write to a read-only "
           "file)."));
  else if (not correct())
    emit settings_warning(
        tr("A format error occurred (e.g. loading a malformed file)."));
}

/*!
 * @fn Basis::read_source_list
 * @brief Reads the store and loads a list of connected sources.
 */
void Basis::read_source_list() {
  auto *store = new QFile(
      get_settings_path() + QDir::separator() + sourcesStoreFilename, this);
  QJsonArray sources_json = read_json(store);
  for (const QJsonValue &source_json : qAsConst(sources_json)) {
    /*!
     * Some properties of sources are stored directly in the database itself
     * in tables. Basis reads them too.
     * @sa SQLite::load
     */
    Source source = sql->load(to_source(source_json.toObject()));
    if (not _sources.contains(source))
      _sources.append(source);
  }
}

/*!
 * @fn Basis::read_message_history
 * @brief Recreates message history from file.
 * @param file QFile to read message history from
 * @returns list of messages from file
 */
QList<Message> Basis::read_message_history(QFile *file) {
  QJsonArray messages_json = read_json(file);
  QList<Message> message_history;
  for (const QJsonValue &obj : qAsConst(messages_json)) {
    Message message = to_message(obj.toObject());
    message_history.append(message);
  }
  return message_history;
}

/*!
 * @fn Basis::write
 * @brief Sets the value of the parameter.
 * @param key parameter name
 * @param data parameter value
 */
void Basis::write(const QString &key, const QVariant &data) {
  if (not correct()) {
    /*!
     * If the file is incorrectly formatted, Basis will not be able to restore
     * the data structure, so it clears the file.
     */
    _settings->clear();
    _settings->sync();
  }
  _settings->setValue(key, data);
}

/*!
 * @fn Basis::write_source_list
 * @brief Writes @a sourceList to @a savefile.
 * @param source_list list of sources' properties
 */
void Basis::write_source_list(QList<Source> source_list) {
  _sources = source_list;
  QJsonArray cs;
  for (const auto &source : qAsConst(source_list)) {
    cs.append(to_json(source));
    // Some properties of containers are stored directly in the database itself
    // in "tables". ASW writes them there.
    sql->write(source);
  }
  auto *savefile =
      new QFile(get_settings_path() + QDir::separator() + sourcesStoreFilename);
  write_json(savefile, cs);
}

/*!
 * @fn Basis::write_message_history
 * @brief Saves @a message_history to @a file.
 * @param message_history list of messages
 * @param file file to save there
 */
void Basis::write_message_history(QList<Message> message_history, QFile *file) {
  QJsonArray message_history_json;
  for (const auto &message : qAsConst(message_history))
    message_history_json.append(to_json(message));
  write_json(file, message_history_json);
}

/*!
 * @fn Basis::read_json
 * @brief Universal JSON read function.
 * @details Additionally checks the file for errors.
 * @param file file to read
 * @returns QJsonArray read from file
 */
QJsonArray Basis::read_json(QFile *file) {
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
 * @fn Basis::write_json
 * @brief Writes @a jsonArray to @a savefile.
 * @details Additionally checks the file for access.
 * @param savefile file to save there
 * @param json_array array to write in @a savefile
 */
void Basis::write_json(QFile *savefile, QJsonArray json_array) {
  if (not savefile->open(QIODevice::WriteOnly | QIODevice::Text))
    return;
  QJsonDocument jsonDocument(json_array);
  QTextStream textStream(savefile);
  textStream << jsonDocument.toJson(QJsonDocument::Indented);
  savefile->close();
}

/*!
 * @fn Basis::to_json
 * @brief Turns @a source into a JSON object.
 * @param source source parameters
 * @returns converted properties of @a source
 */
QJsonObject Basis::to_json(const Source &source) {
  return {{"container", source.table_name},
          {"disabled", source.is_disabled},
          {"path", source.path},
          {"title", source.table_title}};
}

/*!
 * @fn Basis::to_json
 * @brief Turns @a message into a JSON object.
 * @param message message data
 * @returns converted properties of @a message
 */
QJsonObject Basis::to_json(const Message &message) {
  return {{"content", message.content},
          {"datetime", message.datetime.toString(Qt::ISODateWithMs)},
          {"author", int(message.author)},
          {"contentType", int(message.content_type)},
          {"theme", int(message.theme)}};
}

/*!
 * @fn Basis::to_source
 * @brief Turns @a json_object into a source.
 * @param json_object source in JSON
 * @returns source properties
 * @sa Source
 */
Source Basis::to_source(const QJsonObject &json_object) {
  Source source;
  source.table_name = json_object["container"].toString();
  source.is_disabled = json_object["disabled"].toBool();
  source.path = json_object["path"].toString();
  source.table_title = json_object["title"].toString();
  return source;
}

/*!
 * @fn Basis::to_message
 * @brief Turns @a json_object into a message.
 * @param json_object message in JSON
 * @returns message
 * @sa Message
 */
Message Basis::to_message(const QJsonObject &json_object) {
  Message message;
  message.content = json_object["content"].toString();
  message.datetime = QDateTime::fromString(
      json_object["datetime"].toString(), Qt::ISODateWithMs);
  message.author = Author(json_object["author"].toInt());
  message.content_type = ContentType(json_object["contentType"].toInt());
  message.theme = Theme(json_object["theme"].toInt());
  return message;
}
