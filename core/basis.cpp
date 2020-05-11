#include "basis.h"

/*
 * All short named variables and their explanations:
 * {s} <- settings
 * {cfn} <- name of container file
 * {f} <- file
 * {sf} <- savefile
 * {arr} <- array
 * {cProp} <- container properties
 * {cProps} <- list of containers properties
 * {cs} <- container properties in JSON format
 * {sq} <- sqlite
 * {ms} <- messages in JSON format
 * {mh} <- message history
 * {m} <- message
 * {ts} <- text stream
 * {jd} <- JSON-document
 * {arr} <- JSON-array
 * {obj} <- JSON-object
 * {err} <- error
 */

/*!
 * Argument: QObject {*parent}.
 * Checks settings file {s} for any errors.
 */
Basis::Basis(QObject *parent) : QObject(parent) {
  switch (s->status()) {
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
  for (auto obj : cs) {
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
QList<message> Basis::readMessageHistory(QFile *file) {
  QJsonArray ms = readJson(file);
  QList<message> mh;
  for (auto obj : ms) {
    message m = toMessage(obj.toObject());
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
    s->clear();
    s->sync();
    correct = true;
  }
  s->setValue(key, data);
}

/*!
 * Argument: QList of containers properties {containerList} [list for writing].
 * Writes {containerList} to a file {sf}.
 */
void Basis::writeSourceList(QList<Source> sourceList) {
  cProps = sourceList;
  QJsonArray cs;
  for (const auto &cProp : sourceList) {
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
 *            QFile {*savefile}.
 * Saves {messageHistory} to {savefile}.
 */
void Basis::writeMessageHistory(QList<message> messageHistory,
                                QFile *savefile) {
  QJsonArray ms;
  for (const auto &m : messageHistory)
    ms.append(toJSON(m));
  writeJson(savefile, ms);
}

/*!
 * Argument: QFile {*f} [file to read].
 * Universal JSON read function. Checks {f} for any errors.
 * Returns: QJsonArray from file.
 */
QJsonArray Basis::readJson(QFile *f) {
  if (!f->open(QIODevice::ReadOnly | QIODevice::Text))
    return QJsonArray();
  QTextStream ts(f);
  auto *err = new QJsonParseError();
  QJsonDocument jd = QJsonDocument::fromJson(ts.readAll().toUtf8(), err);
  if (err->error != QJsonParseError::NoError) {
    emit jsonError(err->errorString());
    delete err;
    return QJsonArray();
  }
  delete err;
  f->close();
  return jd.array();
}

/*!
 * Arguments: QFile {*sf} [savefile],
 *            QJsonArray {arr} [array to write].
 * Checks {sf} for errors. Writes {arr} to a file {sf}.
 */
void Basis::writeJson(QFile *sf, QJsonArray arr) {
  if (!sf->open(QIODevice::WriteOnly | QIODevice::Text))
    return;
  QJsonDocument jd(arr);
  QTextStream st(sf);
  st << jd.toJson(QJsonDocument::Indented);
  sf->close();
}

/*!
 * Argument: container {cProp} [container properties].
 * Turns {cProp} into a JSON object.
 * Returns: QJsonObject - converted properties of container.
 */
QJsonObject Basis::toJSON(const Source &cProp) {
  return {{"container", cProp.tableName},
          {"disabled", cProp.isDisabled},
          {"path", cProp.path},
          {"title", cProp.tableTitle}};
}

/*!
 * Argument: message {shadow}.
 * Turns {shadow} into a JSON object.
 * Returns: QJsonObject - converted message.
 */
QJsonObject Basis::toJSON(const message &shadow) {
  return {{"content", shadow.content},
          {"datetime", shadow.datetime.toString(Qt::ISODateWithMs)},
          {"author", int(shadow.aType)},
          {"contentType", int(shadow.cType)},
          {"theme", int(shadow.tType)}};
}

/*!
 * Argument: QJsonObject {obj} [JSON container properties].
 * Turns {obj} into a container.
 * Returns: container properties {cProp}.
 */
Source Basis::toSource(const QJsonObject &obj) {
  Source cProp;
  cProp.tableName = obj.value("container").toString();
  cProp.isDisabled = obj.value("disabled").toBool();
  cProp.path = obj.value("path").toString();
  cProp.tableTitle = obj.value("title").toString();
  return cProp;
}

/*!
 * Argument: QJsonObject {obj} [JSON message].
 * Turns {obj} into a message.
 * Returns: message {shadow}.
 */
message Basis::toMessage(const QJsonObject &obj) {
  message shadow;
  shadow.content = obj.value("content").toString();
  shadow.datetime = QDateTime::fromString(obj.value("datetime").toString(),
                                          Qt::ISODateWithMs);
  shadow.aType = eA(obj.value("author").toInt());
  shadow.cType = eC(obj.value("contentType").toInt());
  shadow.tType = eT(obj.value("theme").toInt());
  return shadow;
}
