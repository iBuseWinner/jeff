#ifndef EXTENSION_H
#define EXTENSION_H

#include <QFile>
#include <QHostAddress>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QMap>

/*! @class ExtensionMeta
 *  @brief Contains metadata about an extension.  */
class ExtensionMeta {
public:
  // Functions desribed in `extension.cpp`:
  ExtensionMeta();
  ExtensionMeta(const QJsonObject &json_object);
  QJsonObject to_json() const;
  QString to_string() const;
  static ExtensionMeta *from_string(QString string);
  static ExtensionMeta *from_origin(const QString &origin, bool enabled);

  // Objects:
  bool valid = false;
  QString origin;
  QString name;
  QString desc;
  QMap<QString, QMap<QString, QString>> authors;
  QString license;
  QStringList links;
  QString working_dir;
  QString program;
  QMap<QString, QString> envs;
  QStringList args;
  bool is_server = false;
  QHostAddress server_addr;
  quint16 server_port;
  bool notify_when_no_suggestions = false;
  bool enabled = true;
};

/*! @brief Container for scripts of different types. */
typedef QList<ExtensionMeta *> ExtensionsMeta;

#endif
