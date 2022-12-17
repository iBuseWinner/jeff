#ifndef DAEMONPROCESS_H
#define DAEMONPROCESS_H

#include "core/model/python/script.h"
#include <QFileInfo>
#include <QProcess>
#include <QStringList>

class DaemonProcess : public QProcess {
  Q_OBJECT
  Q_DISABLE_COPY(DaemonProcess)
public:
  // Functions described in `daemon-process.cpp`:
  DaemonProcess(ScriptMetadata *_script, QObject *parent = nullptr);
  void start();
  void stop(int msecs = 1500);
  /*! @brief Tells if this process is spawned by given script. */
  bool is_spawner(ScriptMetadata *s) { return script == s; }

signals:
  /*! @brief Notifies about execution error. */
  QString script_exception(QString error);

private:
  // Objects:
  ScriptMetadata *script = nullptr;
};

#endif
