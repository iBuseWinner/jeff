#ifndef EXTENSION_H
#define EXTENSION_H

#include "yelloger.h"
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
  /*! @brief inner props */
  bool valid = false;
  bool is_server = false;
  bool enabled = true;
  QString origin;
  /*! @brief props from file */
  QString name;
  QString title;
  QString desc;
  QMap<QString, QMap<QString, QString>> authors;
  QString license;
  QStringList links;
  QString working_dir;
  QString program;
  QMap<QString, QString> envs;
  QStringList args;
  QHostAddress server_addr;
  quint16 server_port;
  bool always_send = false;
};

/*! @brief Container for scripts of different types. */
typedef QList<ExtensionMeta *> ExtensionsMeta;

#endif
