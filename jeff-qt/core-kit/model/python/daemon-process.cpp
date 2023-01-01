#include "daemon-process.h"

/*! @brief The constructor. */
DaemonProcess::DaemonProcess(ScriptMetadata *_script, QObject *parent)
  : QProcess(parent), script(_script)
{
  connect(this, &QProcess::errorOccurred, this, [this]() {
    emit script_exception(
      tr("An error occurred during script execution") + " (" + script->path + ")"
    );
  });
  QFileInfo output(script->path);
  setWorkingDirectory(output.path());
}

/*! @brief Starts the daemon. */
void DaemonProcess::start() {
  if (script->path.endsWith(".py")) QProcess::start(QString("python"), QStringList(script->path));
  else QProcess::start(QString(script->path), QStringList());
}

/*! @brief Stops the daemon */
void DaemonProcess::stop(int msecs) {
  disconnect(this, &QProcess::errorOccurred, nullptr, nullptr);
  terminate();
  waitForFinished(msecs);
}
