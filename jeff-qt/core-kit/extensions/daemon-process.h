#ifndef DAEMONPROCESS_H
#define DAEMONPROCESS_H

#include "core-kit/basis.h"
#include "core-kit/extensions/extension.h"
#include "core-kit/extensions/script.h"
#include "core-kit/model/nlp/expression.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QProcess>
#include <QProcessEnvironment>
#include <QRegExp>
#include <QStringList>

class DaemonProcess : public QProcess {
  Q_OBJECT
  Q_DISABLE_COPY(DaemonProcess)
public:
  // Functions described in `daemon-process.cpp`:
  DaemonProcess(ExtensionMeta *_extension_meta, Basis *_basis, QObject *parent = nullptr);
  void stop(int msecs = 1500);
  /*! @brief Tells if this process is spawned by given extension' metadata. */
  bool is_spawner(ExtensionMeta *e) { return extension_meta == e; }
  static QJsonObject request_output(
    ScriptMeta *script_meta, const Expression &expression, const QString &input
  );

signals:
  /*! @brief Notifies about execution error. */
  QString daemon_exception(QString error);

private:
  // Objects:
  ExtensionMeta *extension_meta = nullptr;
  Basis *basis = nullptr;
};

#endif
