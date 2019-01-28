#include "sqlite.h"

void sqlite::create_db(containerProperties newCP) {
  // Creates a database if it does not exist, and a new container if it does not
  // exist either.
  QSqlDatabase NewDB = QSqlDatabase::database();
  NewDB.setDatabaseName(newCP.path);
  NewDB.open();
  QSqlQuery Query(NewDB);
  Query.exec(
      QString("CREATE TABLE %1(address INTEGER, expression TEXT, links TEXT)")
          .arg(newCP.container));
  Query.exec("CREATE TABLE IF NOT EXISTS tables(container TEXT, title TEXT, "
             "readOnly BOOL, private BOOL, catching BOOL)");
  Query.prepare("INSERT INTO tables VALUES (:cnt, :ttl, :ro, :prv, :ctch)");
  Query.bindValue(":cnt", newCP.container);
  Query.bindValue(":ttl", newCP.title);
  Query.bindValue(":ro", newCP.readOnly);
  Query.bindValue(":prv", newCP.isPrivate);
  Query.bindValue(":ctch", newCP.isCatching);
  Query.exec();
  NewDB.close();
}

QStringList sqlite::containers(const QString& Path) {
  // Returns list of existing containers.
  QSqlDatabase DB = QSqlDatabase::database();
  DB.setDatabaseName(Path);
  DB.open();
  QStringList Containers = DB.tables();
  for (int i = 0; i < Containers.length(); i++) {
    QSqlRecord Record = DB.record(Containers.at(i));
    if ((Record.fieldName(0) != "address") ||
        (Record.fieldName(1) != "expression") ||
        (Record.fieldName(2) != "links")) {
      Containers.removeAt(i);
      // Now all containers go back for one element, and thus...
      i--;
    }
  }
  DB.close();
  return Containers;
}

containerProperties sqlite::optionsLoader(containerProperties container) {
  QSqlDatabase DB = QSqlDatabase::database();
  DB.setDatabaseName(container.path);
  DB.open();
  QSqlQuery Query(DB);
  Query.prepare(
      QString("SELECT (title, ro, private, catching) FROM tables WHERE "
              "container=:cont"));
  Query.bindValue(":cont", container.container);
  Query.exec();
  if (!Query.first())
    return container;
  container.title = Query.value(0).toString();
  container.readOnly = Query.value(1).toBool();
  container.isPrivate = Query.value(2).toBool();
  container.isCatching = Query.value(3).toBool();
  DB.close();
  return container;
}

void sqlite::optionsWriter(containerProperties container) {
  QSqlDatabase DB = QSqlDatabase::database();
  DB.setDatabaseName(container.path);
  DB.open();
  QSqlQuery Query(DB);
  Query.exec("CREATE TABLE IF NOT EXISTS tables(container TEXT, title TEXT, "
             "readOnly BOOL, private BOOL, catching BOOL)");
  Query.prepare("DELETE FROM tables WHERE container=:cnt");
  Query.bindValue(":cnt", container.container);
  Query.exec();
  Query.prepare("INSERT INTO tables VALUES(:cnt, :ttl, :ro, :prv, :ctch)");
  Query.bindValue(":cnt", container.container);
  Query.bindValue(":ttl", container.title);
  Query.bindValue(":ro", container.readOnly);
  Query.bindValue(":prv", container.isPrivate);
  Query.bindValue(":ctch", container.isCatching);
  Query.exec();
  DB.close();
}

void sqlite::insert(const QString& Path, const QString& Container, int Address,
                    const QString& Expression, const QString& Links) {
  // Inserts a new entry into the container.
  QSqlDatabase DB = QSqlDatabase::database();
  DB.setDatabaseName(Path);
  DB.open();
  QSqlQuery Query(DB);
  Query.prepare(QString("INSERT OR REPLACE INTO %1 VALUES (:add, :exp, :lnk)")
                    .arg(Container));
  Query.bindValue(":add", Address);
  Query.bindValue(":exp", Expression);
  Query.bindValue(":lnk", Links);
  Query.exec();
  DB.close();
}

QPair<QString, QString> sqlite::scan(const QString& Path, const QString& Container,
                                     int Address) {
  // Returns expression and links by address value.
  QSqlDatabase DB = QSqlDatabase::database();
  DB.setDatabaseName(Path);
  DB.open();
  QSqlQuery Query(DB);
  Query.prepare(QString("SELECT expression, links FROM %1 WHERE address = :add")
                    .arg(Container));
  Query.bindValue(":add", Address);
  Query.exec();
  QString Expressions;
  QString Links;
  Query.first();
  while (Query.isValid()) {
    Expressions = Query.value(0).toString();
    Links = Query.value(1).toString();
    Query.next();
  }
  DB.close();
  return QPair<QString, QString>(Expressions, Links);
}

bool sqlite::hasAdditionalProperties(const QString& Path, const QString& Container) {
  // Find out if the table has additional properties.
  QSqlDatabase DB = QSqlDatabase::database();
  DB.setDatabaseName(Path);
  DB.open();
  QSqlRecord Record = DB.record(Container);
  DB.close();
  return Record.fieldName(3) != "";
}

QMap<QString, QString> sqlite::scanAP(const QString& Path, const QString& Container,
                                      int Address) {
  // Returns additional table properties.
  QSqlDatabase DB = QSqlDatabase::database();
  DB.setDatabaseName(Path);
  DB.open();
  QSqlQuery Query(DB);
  Query.prepare(
      QString("SELECT * FROM %1 WHERE address = :add").arg(Container));
  Query.bindValue(":add", Address);
  Query.exec();
  DB.close();
  QSqlRecord Record = Query.record();
  int field = 3;
  QMap<QString, QString> additionalProperties;
  while (!Record.isNull(field))
    additionalProperties.insert(Record.fieldName(field),
                                Record.value(field).toString());
  return additionalProperties;
}

QList<QPair<QString, QString>> sqlite::scan(const QString& Path, const QString& Container,
                                            const QString& Expression) {
  // Returns activators and links by expression value.
  QSqlDatabase DB = QSqlDatabase::database();
  DB.setDatabaseName(Path);
  DB.open();
  handlers HD;
  QSqlQuery Query(DB);
  Query.exec(QString("SELECT expression, links FROM %1").arg(Container));
  QList<QPair<QString, QString>> activatorsAndLinks;
  Query.first();
  while (Query.isValid()) {
    if (Expression.contains(HD.purifyString(Query.value(0).toString()),
                            Qt::CaseInsensitive))
      activatorsAndLinks.append(QPair<QString, QString>(
          Query.value(0).toString(), Query.value(1).toString()));
    Query.next();
  }
  DB.close();
  return activatorsAndLinks;
}
