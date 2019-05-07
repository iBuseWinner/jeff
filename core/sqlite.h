#ifndef SQLITE_H
#define SQLITE_H

#include <QFile>
#include <QJsonObject>
#include <QMap>
#include <QObject>
#include <QPair>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlResult>
#include <QString>
#include <QStringList>
#include <QVariant>
#include "core/container.h"
#include "core/handlers.h"

class sqlite : public QObject {
  Q_OBJECT
 public:
  sqlite(QObject *p = nullptr);
  void create(const container &cProp);
  QList<container> containers(const QString &p);
  container optionsLoader(container cProp);
  void optionsWriter(const container &cProp);
  void insert(const container &cProp, int a, const QString &e,
              const QString &ls);
  QPair<QString, QString> scan(const container &cProp, int a);
  bool hasAdditionalProperties(const container &cProp);
  QMap<QString, QString> scanAP(const container &cProp, int a);
  QMap<QString, QString> scan(const container &cProp, const QString &e);

 signals:
  QString sqliteError(QString et);
  QString sqliteWarning(QString wt);

 private:
  void createMainTable(QSqlQuery q);
  bool exists(const QString &p);
  QPair<QSqlDatabase, bool> openDB(const QString &p);
  bool isDBEmpty(QSqlDatabase _db);
  QSqlQuery execQuery(QSqlQuery q);
};

#endif  // SQLITE_H
