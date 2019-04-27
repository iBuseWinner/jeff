#ifndef SQLITE_H
#define SQLITE_H

#include <QMap>
#include <QPair>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlResult>
#include <QString>
#include <QStringList>
#include <QVariant>
#include "core/containersstruct.h"
#include "core/handlers.h"

class sqlite {
 public:
  void create(const containerProperties &ncp);
  QList<containerProperties> containers(const QString &p);
  containerProperties optionsLoader(containerProperties cp);
  void optionsWriter(const containerProperties &cp);
  void insert(const QString &p, const QString &c, int a, const QString &ex,
              const QString &ls);
  QPair<QString, QString> scan(const QString &p, const QString &c, int a);
  bool hasAdditionalProperties(const QString &p, const QString &c);
  QMap<QString, QString> scanAP(const QString &p, const QString &c, int a);
  QList<QPair<QString, QString>> scan(const QString &p, const QString &c,
                                      const QString &ex);
  QList<QPair<QString, QString>> paths();

 private:
  void ct(QSqlQuery q);
};

#endif  // SQLITE_H
