#include "daemon-process.h"

/*! @brief The constructor. */
DaemonProcess::DaemonProcess(ExtensionMeta *_extension_meta, Basis *_basis, QObject *parent)
  : QProcess(parent), basis(_basis), extension_meta(_extension_meta)
{
  setProgram(extension_meta->program);
  QStringList args = extension_meta->args;
  for (int i = 0; i < args.length(); i++) {
    if (args[i] == "<JEFF_PORT>") {
      if (not (*basis)[basis->serverPortSt].toString().isEmpty())
        args[i] = (*basis)[basis->serverPortSt].toString();
      else args[i] = QString::number(8005);
    }
  }
  setArguments(args);
  if (not extension_meta->working_dir.isEmpty()) setWorkingDirectory(extension_meta->working_dir);
  if (not extension_meta->envs.isEmpty()) {
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    for (auto key : extension_meta->envs.keys()) env.insert(key, extension_meta->envs[key]);
    setProcessEnvironment(env);
  }
  connect(this, &QProcess::errorOccurred, this, [this]() {
    QString cmd = extension_meta->program;
    for (auto arg : extension_meta->args) cmd += QString(" ") + arg;
    emit daemon_exception(
      tr("An error occurred during daemon execution") + " (\"`" + cmd + "\"`)."
    );
  });
}

/*! @brief Stops the daemon. */
void DaemonProcess::stop(int msecs) {
  disconnect(this, &QProcess::errorOccurred, nullptr, nullptr);
  terminate();
  waitForFinished(msecs);
}

/*! @brief Requests output from external program. */
QJsonObject DaemonProcess::request_output(
  ScriptMeta *script_meta, const Expression &expression, const QString &input
) {
  QProcess process;
  process.setProgram(script_meta->program);
  process.setArguments(script_meta->args);
  if (not script_meta->working_dir.isEmpty()) process.setWorkingDirectory(script_meta->working_dir);
  if (not script_meta->envs.isEmpty()) {
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    for (auto key : script_meta->envs.keys()) env.insert(key, script_meta->envs[key]);
    process.setProcessEnvironment(env);
  }
  QJsonObject json_payload = {
    {"expression", expression.to_json()},
    {"input", input}
  };
  QJsonDocument payload_doc(json_payload);
  process.start();
  process.waitForStarted(500);
  process.write(payload_doc.toJson(QJsonDocument::Compact));
  if (not process.waitForFinished(10000)) process.kill();
  if (not process.canReadLine()) return QJsonObject();
  QJsonParseError errors;
  QJsonDocument out_doc = QJsonDocument::fromJson(process.readAll(), &errors);
  if (errors.error != QJsonParseError::NoError) return QJsonObject();
  return out_doc.object();
}
