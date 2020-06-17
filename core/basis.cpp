#include "basis.h"

/*!
 * Argument: QObject {*parent}.
 * Checks settings file for any errors.
 */
void Basis::check() {
  switch (settings->status()) {
  case QSettings::AccessError:
    emit settingsWarning(
        tr("An access error occurred (e.g. trying to write to a read-only "
           "file)."));
    access = false;
    break;
  case QSettings::FormatError:
    emit settingsWarning(
        tr("A format error occurred (e.g. loading a malformed file)."));
    correct = false;
    break;
  default:;
  }
}

/*!
 * Compiles a list of sources used by the ASW.
 */
void Basis::readSourceList() {
  auto *store = new QFile(
      settingsPath() + QDir::separator() + sourcesStoreFilename, this);
  QJsonArray sources_json = readJson(store);
  for (const QJsonValue &source_json : qAsConst(sources_json)) {
    // Some properties of sources are stored directly in the database itself
    // in "tables". ASW reads them too {sql->optionsLoader()}.
    Source source = sql->load(toSource(source_json.toObject()));
    if (not sources.contains(source))
      sources.append(source);
  }
}

/*!
 * Argument: QFile {*file} [file to read].
 * Recreates message history from file.
 * Returns: QList of messages {message_history}.
 */
QList<Message> Basis::readMessageHistory(QFile *file) {
  QJsonArray messages_json = readJson(file);
  QList<Message> message_history;
  for (const QJsonValue &obj : qAsConst(messages_json)) {
    Message message = toMessage(obj.toObject());
    message_history.append(message);
  }
  return message_history;
}

/*!
 * Arguments: QString {key} [parameter name],
 *            QVariant {data} [parameter value].
 * Sets the value of the parameter.
 */
void Basis::write(const QString &key, const QVariant &data) {
  // If the file is incorrectly formatted, ASW will not be able to restore the
  // data structure, so it clears the file.
  if (not correct) {
    settings->clear();
    settings->sync();
    correct = true;
  }
  settings->setValue(key, data);
}

/*!
 * Argument: QList of sources properties {sourceList} [list for writing].
 * Writes {sourceList} to a file {savefile}.
 */
void Basis::writeSourceList(QList<Source> sourceList) {
  sources = sourceList;
  QJsonArray cs;
  for (const auto &source : qAsConst(sourceList)) {
    cs.append(toJSON(source));
    // Some properties of containers are stored directly in the database itself
    // in "tables". ASW writes them there.
    sql->write(source);
  }
  auto *savefile =
      new QFile(settingsPath() + QDir::separator() + sourcesStoreFilename);
  writeJson(savefile, cs);
}

/*!
 * Arguments: QList of messages {message_history} [history to save],
 *            QFile {*file}.
 * Saves {message_history} to {file}.
 */
void Basis::writeMessageHistory(QList<Message> message_history, QFile *file) {
  QJsonArray message_history_json;
  for (const auto &message : qAsConst(message_history))
    message_history_json.append(toJSON(message));
  writeJson(file, message_history_json);
}

/*!
 * Argument: QFile {*file} [file to read].
 * Universal JSON read function. Checks {file} for any errors.
 * Returns: QJsonArray from file.
 */
QJsonArray Basis::readJson(QFile *file) {
  if (not file->open(QIODevice::ReadOnly | QIODevice::Text))
    return QJsonArray();
  QTextStream textStream(file);
  auto *errors = new QJsonParseError();
  QJsonDocument document =
      QJsonDocument::fromJson(textStream.readAll().toUtf8(), errors);
  if (errors->error != QJsonParseError::NoError) {
    emit jsonError(errors->errorString());
    delete errors;
    return QJsonArray();
  }
  delete errors;
  file->close();
  return document.array();
}

/*!
 * Arguments: QFile {*savefile} [savefile],
 *            QJsonArray {arr} [array to write].
 * Checks {savefile} for errors. Writes {jsonArray} to a file {savefile}.
 */
void Basis::writeJson(QFile *savefile, QJsonArray jsonArray) {
  if (not savefile->open(QIODevice::WriteOnly | QIODevice::Text))
    return;
  QJsonDocument jsonDocument(jsonArray);
  QTextStream textStream(savefile);
  textStream << jsonDocument.toJson(QJsonDocument::Indented);
  savefile->close();
}

/*!
 * Argument: container {cProp} [container properties].
 * Turns {cProp} into a JSON object.
 * Returns: QJsonObject - converted properties of container.
 */
QJsonObject Basis::toJSON(const Source &source) {
  return {{"container", source.tableName},
          {"disabled", source.isDisabled},
          {"path", source.path},
          {"title", source.tableTitle}};
}

/*!
 * Argument: message {shadow}.
 * Turns {shadow} into a JSON object.
 * Returns: QJsonObject - converted message.
 */
QJsonObject Basis::toJSON(const Message &message) {
  return {{"content", message.content},
          {"datetime", message.datetime.toString(Qt::ISODateWithMs)},
          {"author", int(message.aType)},
          {"contentType", int(message.cType)},
          {"theme", int(message.tType)}};
}

/*!
 * Argument: QJsonObject {obj} [JSON container properties].
 * Turns {obj} into a container.
 * Returns: container properties {cProp}.
 */
Source Basis::toSource(const QJsonObject &jsonObject) {
  Source source;
  source.tableName = jsonObject.value("container").toString();
  source.isDisabled = jsonObject.value("disabled").toBool();
  source.path = jsonObject.value("path").toString();
  source.tableTitle = jsonObject.value("title").toString();
  return source;
}

/*!
 * Argument: QJsonObject {obj} [JSON message].
 * Turns {obj} into a message.
 * Returns: message {shadow}.
 */
Message Basis::toMessage(const QJsonObject &jsonObject) {
  Message shadow;
  shadow.content = jsonObject.value("content").toString();
  shadow.datetime = QDateTime::fromString(
      jsonObject.value("datetime").toString(), Qt::ISODateWithMs);
  shadow.aType = Author(jsonObject.value("author").toInt());
  shadow.cType = ContentType(jsonObject.value("contentType").toInt());
  shadow.tType = Theme(jsonObject.value("theme").toInt());
  return shadow;
}
