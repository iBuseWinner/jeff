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
  QStringList containers(const QString& Path);
  containerProperties optionsLoader(containerProperties container);
  void optionsWriter(containerProperties container);
  void insert(const QString& Path, const QString& Container, int Address, const QString& Expression,
              const QString& Links);
  QPair<QString, QString> scan(const QString& Path, const QString& Container, int Address);
  bool hasAdditionalProperties(const QString& Path, const QString& Container);
  QMap<QString, QString> scanAP(const QString& Path, const QString& Container, int Address);
  QList<QPair<QString, QString>> scan(const QString& Path, const QString& Container,
                                      const QString& Expression);
  QList<QPair<QString, QString>> paths();
};

#endif // SQLITE_H
