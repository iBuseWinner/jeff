#ifndef DAEMONPROCESS_H
#define DAEMONPROCESS_H

#include "core-kit/basis.hpp"
#include "core-kit/extensions/extension.hpp"
#include "core-kit/extensions/script.hpp"
#include "core-kit/model/nlp/expression.hpp"
#include "yelloger.h"
#include <QFileInfo>
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
  DaemonProcess(Basis *_basis, ExtensionMeta *_extension_meta, QObject *parent = nullptr);
  void stop(int msecs = 5000);
  /*! @brief Tells if this process is spawned by given extension' metadata. */
  bool is_spawner(ExtensionMeta *e) { return extension_meta == e; }
  static QJsonObject request_output(
    ScriptMeta *script_meta, const Expression &expression, const QString &input
  );
  QByteArray get_output();

signals:
  /*! @brief Notifies about execution error. */
  QString daemon_exception(QString error);

private:
  // Objects:
  Basis *basis = nullptr;
  ExtensionMeta *extension_meta = nullptr;
};

#endif
