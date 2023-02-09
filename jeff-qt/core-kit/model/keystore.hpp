#ifndef KEYSTORE_H
#define KEYSTORE_H

#include <QJsonValue>
#include <QMap>
#include <QString>

/*! @typedef KeyStore
 *  @brief Contains a key-value store.  */
typedef QMap<QString, QJsonValue> KeyStore;

#endif
