#include "daemon-process.hpp"

/*! @brief The constructor. */
DaemonProcess::DaemonProcess(Basis *_basis, ExtensionMeta *_extension_meta, QObject *parent)
  : QProcess(parent), basis(_basis), extension_meta(_extension_meta)
{
  Yellog::Trace("Got extension %p", extension_meta);
  setProgram(extension_meta->program);
  QStringList args = extension_meta->args;
  for (int i = 0; i < args.length(); i++) {
    if (args[i] == "<JEFF_PORT>") {
      if (not (*basis)[Basis::serverPortSt].toString().isEmpty()) {
        args[i] = (*basis)[Basis::serverPortSt].toString();
        Yellog::Trace("\tSetted up port %s.", (*basis)[Basis::serverPortSt].toString().toStdString().c_str());
      } else {
        args[i] = QString::number(8005);
        Yellog::Trace("\tSetted up port 8005");
      }
    } else if (args[i] == "<SERVER_PORT>") {
      if (extension_meta->is_server) {
        args[i] = QString::number(extension_meta->server_port);
        Yellog::Trace("\tSetted up server port given in extension.j.json.");
      }
    }
  }
  setArguments(args);
  if (not extension_meta->working_dir.isEmpty()) {
    setWorkingDirectory(extension_meta->working_dir);
    Yellog::Trace("\tSetted working dir %s", extension_meta->working_dir.toStdString().c_str());
  } else if (not extension_meta->origin.isEmpty()) {
    QFileInfo fi(extension_meta->origin);
    setWorkingDirectory(fi.canonicalPath());
    Yellog::Trace("\tSetted working dir %s", fi.canonicalPath().toStdString().c_str());
  }
  if (not extension_meta->envs.isEmpty()) {
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    for (auto key : extension_meta->envs.keys()) env.insert(key, extension_meta->envs[key]);
    setProcessEnvironment(env);
  }
  connect(this, &QProcess::errorOccurred, this, [this] {
    QString cmd = extension_meta->program;
    for (auto arg : extension_meta->args) cmd += QString(" ") + arg;
    emit daemon_exception(tr("An error occurred during daemon execution") + " (\"" + cmd + "\").");
  });
}

/*! @brief Stops the daemon. */
void DaemonProcess::stop(int msecs) {
  disconnect(this, &QProcess::errorOccurred, nullptr, nullptr);
  Yellog::Trace("Terminating...");
  terminate();
  Yellog::Trace("\tWaiting for finished...");
  waitForFinished(msecs);
}

/*! @brief Requests output from external program. */
QJsonObject DaemonProcess::request_output(
  HProcessor *hp, Basis *basis, ScriptMeta *script_meta, const Expression &expression, const QString &input
) {
  Yellog::Trace("Got extension %p", script_meta);
  QProcess process;
  process.setProgram(script_meta->program);
  process.setArguments(script_meta->args);
  if (not script_meta->working_dir.isEmpty()) {
    process.setWorkingDirectory(script_meta->working_dir);
    Yellog::Trace("\tSetted working dir %s", script_meta->working_dir.toStdString().c_str());
  }
  if (not script_meta->envs.isEmpty()) {
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    for (auto key : script_meta->envs.keys()) env.insert(key, script_meta->envs[key]);
    process.setProcessEnvironment(env);
  }
  QJsonObject transport;
  if (not script_meta->required_memory_cells.isEmpty()) {
    QJsonObject memory_cells;
    for (auto key : script_meta->required_memory_cells) memory_cells[key] = basis->memory(key);
    transport[Basis::memoryValuesWk] = memory_cells;
  }
  if (script_meta->required_history_parts and not (*basis)[Basis::disableMessagesTransmissionSt].toBool()) {
    QJsonArray history_array;
    auto history = hp->recent(script_meta->required_history_parts);
    for (auto msg : history) 
      history_array.append(
        QString("%1: %2").arg(msg->author == Author::User ? "User" : "Jeff").arg(msg->content)
      );
    transport[Basis::recentMessagesWk] = history_array;
  }
  if (script_meta->required_user_input) transport["user_input"] = input;
  if (not expression.properties.isEmpty()) {
    transport[Basis::exprPropsWk] = Phrase::pack_props(expression.properties);
  }
  QJsonDocument payload_doc(transport);
  process.start(QProcess::Unbuffered | QProcess::ReadWrite);
  process.waitForStarted(500);
  Yellog::Trace("\tProcess must be started at this point.");
  Yellog::Trace("\tWriting payload and waiting for finished...");
  process.write(payload_doc.toJson(QJsonDocument::Compact));
  if (not process.waitForFinished(10000)) {
    Yellog::Info("\tProcess has been killed.");
    process.kill();
  }
  if (not process.canReadLine()) {
    Yellog::Warn("\tNo line given at stdout.");
    return QJsonObject({{Basis::errorTypeWk, 13}});
  }
  QJsonParseError errors;
  QJsonDocument out_doc = QJsonDocument::fromJson(process.readAll(), &errors);
  if (errors.error != QJsonParseError::NoError) {
    Yellog::Error("\tIt's impossible to parse response from JSON. Error int: %d", int(errors.error));
    return QJsonObject({{Basis::errorTypeWk, 7}});
  }
  if (not out_doc.isObject()) {
    Yellog::Error("\tIt's impossible to get object from response JSON. Error int: %d", int(errors.error));
    return {{Basis::errorTypeWk, 8}};
  }
  return out_doc.object();
}

/*! @brief Reads all standard output. */
QByteArray DaemonProcess::get_output() { return readAllStandardOutput(); }

/*! @brief Reads all standard error. */
QByteArray DaemonProcess::get_error() { return readAllStandardError(); }
