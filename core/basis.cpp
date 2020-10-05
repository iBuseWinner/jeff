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
      get_settings_path() + QDir::separator() + sources_store_filename, this);
  QJsonArray sources_json = read_json(store);
  for (const QJsonValue &source_json : qAsConst(sources_json)) {
    /*!
     * Some properties of sources are stored directly in the database itself
     * in tables. Basis reads them too.
     * @sa SQLite::load
     */
    Source source = sql->load_source(to_source(source_json.toObject()));
    if (not _sources.contains(source))
      _sources.append(source);
  }
}

/*!
 * @fn Basis::read_message_history
 * @brief Recreates message history from file.
 * @param[in,out] file QFile to read message history from
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
 * @param[in] key parameter name
 * @param[in] data parameter value
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
 * @param[in] source_list list of sources' properties
 */
void Basis::write_source_list(QList<Source> source_list) {
  _sources = source_list;
  QJsonArray cs;
  for (const auto &source : qAsConst(source_list)) {
    cs.append(to_json(source));
    // Some properties of containers are stored directly in the database itself
    // in "tables". ASW writes them there.
    sql->write_source(source);
  }
  auto *savefile = new QFile(get_settings_path() + QDir::separator() +
                             sources_store_filename);
  write_json(savefile, cs);
}

/*!
 * @fn Basis::write_message_history
 * @brief Saves @a message_history to @a file.
 * @param[in] message_history list of messages
 * @param[in,out] file file to save there
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
 * @param[in,out] file file to read
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
 * @param[in,out] savefile file to save there
 * @param[in] json_array array to write in @a savefile
 */
void Basis::write_json(QFile *savefile, QJsonArray json_array) {
  if (not savefile->open(QIODevice::WriteOnly | QIODevice::Text))
    return;
  QJsonDocument jsonDocument(json_array);
  QTextStream textStream(savefile);
  textStream << jsonDocument.toJson(QJsonDocument::Indented);
  savefile->close();
}
