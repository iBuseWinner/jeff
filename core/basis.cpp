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
 * Compiles a list of containers used by the ASW.
 * Returns: QList of containers properties {cProps}.
 */
QList<Source> Basis::readSourceList() {
  auto *f = new QFile(settingsPath() + QDir::separator() + cfn, this);
  QJsonArray cs = readJson(f);
  for (auto obj : qAsConst(cs)) {
    // Some properties of containers are stored directly in the database itself
    // in "tables". ASW reads them too {sq->optionsLoader()}.
    cProps.append(sql->load(toSource(obj.toObject())));
  }
  return cProps;
}

/*!
 * Argument: QFile {*file} [file to read].
 * Recreates message history from file.
 * Returns: QList of messages {mh}.
 */
QList<Message> Basis::readMessageHistory(QFile *file) {
  QJsonArray ms = readJson(file);
  QList<Message> mh;
  for (auto obj : qAsConst(ms)) {
    Message m = toMessage(obj.toObject());
    mh.append(m);
  }
  return mh;
}

/*!
 * Arguments: QString {key} [parameter name],
 *            QVariant {data} [parameter value].
 * Sets the value of the parameter.
 */
void Basis::write(const QString &key, const QVariant &data) {
  // If the file is incorrectly formatted, ASW will not be able to restore the
  // data structure, so it clears the file.
  if (!correct) {
    settings->clear();
    settings->sync();
    correct = true;
  }
  settings->setValue(key, data);
}

/*!
 * Argument: QList of containers properties {containerList} [list for writing].
 * Writes {containerList} to a file {sf}.
 */
void Basis::writeSourceList(QList<Source> sourceList) {
  cProps = sourceList;
  QJsonArray cs;
  for (const auto &cProp : qAsConst(sourceList)) {
    cs.append(toJSON(cProp));
    // Some properties of containers are stored directly in the database itself
    // in "tables". ASW writes them there.
    sql->write(cProp);
  }
  auto *sf = new QFile(settingsPath() + QDir::separator() + cfn);
  writeJson(sf, cs);
}

/*!
 * Arguments: QList of messages {messageHistory} [history to save],
 *            QFile {*file}.
 * Saves {messageHistory} to {file}.
 */
void Basis::writeMessageHistory(QList<Message> messageHistory, QFile *file) {
  QJsonArray jsonMessageHistory;
  for (const auto &message : qAsConst(messageHistory))
    jsonMessageHistory.append(toJSON(message));
  writeJson(file, jsonMessageHistory);
}

/*!
 * Argument: QFile {*file} [file to read].
 * Universal JSON read function. Checks {file} for any errors.
 * Returns: QJsonArray from file.
 */
QJsonArray Basis::readJson(QFile *file) {
  if (!file->open(QIODevice::ReadOnly | QIODevice::Text))
    return QJsonArray();
  QTextStream textStream(file);
  auto *errors = new QJsonParseError();
  QJsonDocument jd =
      QJsonDocument::fromJson(textStream.readAll().toUtf8(), errors);
  if (errors->error != QJsonParseError::NoError) {
    emit jsonError(errors->errorString());
    delete errors;
    return QJsonArray();
  }
  delete errors;
  file->close();
  return jd.array();
}

/*!
 * Arguments: QFile {*sf} [savefile],
 *            QJsonArray {arr} [array to write].
 * Checks {sf} for errors. Writes {arr} to a file {sf}.
 */
void Basis::writeJson(QFile *file, QJsonArray jsonArray) {
  if (!file->open(QIODevice::WriteOnly | QIODevice::Text))
    return;
  QJsonDocument jsonDocument(jsonArray);
  QTextStream textStream(file);
  textStream << jsonDocument.toJson(QJsonDocument::Indented);
  file->close();
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
