#ifndef SQLITE_H
#define SQLITE_H

#include "core/containersstruct.h"
#include "core/handlers.h"
#include <QMap>
#include <QPair>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlResult>
#include <QSqlError>
#include <QString>
#include <QStringList>
#include <QVariant>

class sqlite {
public:
  void create_db(containerProperties newCP);
  QStringList containers(QString Path);
  containerProperties optionsLoader(containerProperties container);
  void optionsWriter(containerProperties container);
  void insert(QString Path, QString Container, int Address, QString Expression,
              QString Links);
  QPair<QString, QString> scan(QString Path, QString Container, int Address);
  bool hasAdditionalProperties(QString Path, QString Container);
  QMap<QString, QString> scanAP(QString Path, QString Container, int Address);
  QList<QPair<QString, QString>> scan(QString Path, QString Container,
                                      QString Expression);
  QList<QPair<QString, QString>> paths();
};

#endif // SQLITE_H
