#ifndef DAEMONPROCESS_H
#define DAEMONPROCESS_H

#include "core-kit/model/python/script.h"
#include <QFileInfo>
#include <QProcess>
#include <QRegExp>
#include <QStringList>

class DaemonProcess : public QProcess {
  Q_OBJECT
  Q_DISABLE_COPY(DaemonProcess)
public:
  // Functions described in `daemon-process.cpp`:
  DaemonProcess(DaemonizeableScriptMetadata *_script, QObject *parent = nullptr);
  void start();
  void stop(int msecs = 1500);
  /*! @brief Tells if this process is spawned by given script. */
  bool is_spawner(DaemonizeableScriptMetadata *s) { return script == s; }

signals:
  /*! @brief Notifies about execution error. */
  QString script_exception(QString error);

private:
  // Objects:
  DaemonizeableScriptMetadata *script = nullptr;
  QStringList parsed_args;
  QString main_cmd;
};

#endif
