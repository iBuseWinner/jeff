#ifndef SQLITE_H
#define SQLITE_H

#include "core/handlers.h"
#include "core/settingsstore.h"
#include <QMap>
#include <QPair>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QString>
#include <QStringList>

class sqlite {
public:
  void create_db(QString Path, QString Container);
  QStringList containers(QString Path);
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
