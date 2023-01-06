#include "daemon-process.h"

/*! @brief The constructor. */
DaemonProcess::DaemonProcess(DaemonizeableScriptMetadata *_script, QObject *parent)
  : QProcess(parent), script(_script)
{
  /*! Regular expression by @author DaVinci v.3 (OpenAI's ChatGPT). */
  // QRegExp divider("\"([^\"\\\\]*(\\\\.[^\"\\\\]*)*)\"|'([^'\\\\]*(\\\\.[^'\\\\]*)*)'|([^\\s]+)");
  parsed_args = QStringList(script->cmd); //.split(divider);
  main_cmd = parsed_args.first();
  parsed_args.pop_front();
  connect(this, &QProcess::errorOccurred, this, [this]() {
    emit script_exception(
      tr("An error occurred during script execution") + " (\"" + script->cmd + "\")"
    );
  });
  QFileInfo output(main_cmd);
  setWorkingDirectory(output.path());
}

/*! @brief Starts the daemon. */
void DaemonProcess::start() {
  QProcess::start(main_cmd, parsed_args);
}

/*! @brief Stops the daemon */
void DaemonProcess::stop(int msecs) {
  disconnect(this, &QProcess::errorOccurred, nullptr, nullptr);
  terminate();
  waitForFinished(msecs);
}
