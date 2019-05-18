#include "sqlite.h"

/*
 * All short named objects and their explanations:
 * {db} <- database
 * {q} <- query
 * {r} <- record
 * {o} <- option
 * {cProp} <- container properties
 * {cProps} <- containers properties
 * {c} <- container
 * {cntr} <- counter
 * {mna} <- maximum number of attempts
 * {rand} <- randomizer
 * {uuid} <- universally unique identifier
 * {vs} <- values
 */

/*!
 * Argument: container {_container} [container properties].
 * Creates container.
 */
void sqlite::create(const container &_container) {
  // The container is created even if the database itself does not exist.
  QSqlDatabase db = prepare(_container.path, check::OnlyOpen);
  if (db.databaseName() == QString()) return;
  // Generates a table name. User-entered name may be incorrect for SQLite
  // database.
  QString c;
  int cntr = 0;
  QStringList tables = db.tables();
  QRandomGenerator rand(quint32(QTime::currentTime().msec()));
  do {
    c = QUuid(uint(rand.generate()), ushort(rand.generate()),
              ushort(rand.generate()), uchar(rand.bounded(0, 256)),
              uchar(rand.bounded(0, 256)), uchar(rand.bounded(0, 256)),
              uchar(rand.bounded(0, 256)), uchar(rand.bounded(0, 256)),
              uchar(rand.bounded(0, 256)), uchar(rand.bounded(0, 256)),
              uchar(rand.bounded(0, 256)))
            .toString();
    cntr++;
  } while ((tables.contains(c)) && (cntr < mna));
  if (tables.contains(c)) {
    emit sqliteError(
        "Could not create container, number of attempts exceeded " +
        QString(mna) + ".");
    return;
  }
  uuid = c;
  auto *q = new QSqlQuery(db);
  exec(q, todo::CreateMainTable);
  QStringList vs;
  vs.append(uuid);
  vs.append(_container.tableTitle);
  vs.append(QString(_container.isReadOnly));
  vs.append(QString(_container.isPrivate));
  vs.append(QString(_container.isCatching));
  exec(q, todo::WriteOptions, vs);
  exec(q, todo::CreateContainerTable, QStringList(uuid));
  db.close();
  delete q;
}

/*!
 * Argument: QString {path} [path to database].
 * Searches for database containers of activators and reagents.
 * Returns: QList of containers {cProps}.
 */
QList<container> sqlite::containers(const QString &path) {
  QSqlDatabase db = prepare(path, check::AnyContent);
  if (db.databaseName() == QString()) return QList<container>();
  auto *q = new QSqlQuery(db);
  exec(q, todo::SelectContainers);
  db.close();
  q->first();
  QList<container> cProps;
  while (q->isValid()) {
    container cProp;
    cProp.path = path;
    cProp.tableName = q->value(0).toString();
    cProp.tableTitle = q->value(1).toString();
    cProp.isReadOnly = q->value(2).toInt();
    cProp.isPrivate = q->value(3).toInt();
    cProp.isCatching = q->value(4).toInt();
    cProps.append(cProp);
    q->next();
  }
  delete q;
  return cProps;
}

/*!
 * Argument: container {_container} [container properties].
 * Loads container properties from the database.
 * Returns: container properties {_container}.
 */
container sqlite::load(container _container) {
  QSqlDatabase db = prepare(_container.path);
  if (db.databaseName() == QString()) return _container;
  auto *q = new QSqlQuery(db);
  exec(q, todo::LoadOptions, QStringList(_container.tableName));
  db.close();
  if (!q->first()) return _container;
  _container.tableTitle = q->value(0).toString();
  _container.isReadOnly = q->value(1).toBool();
  _container.isPrivate = q->value(2).toBool();
  _container.isCatching = q->value(3).toBool();
  delete q;
  return _container;
}

/*!
 * Argument: container {_container} [container properties].
 * Writes container properties into the database.
 */
void sqlite::write(const container &_container) {
  QSqlDatabase db = prepare(_container.path, check::AnyContent);
  if (db.databaseName() == QString()) return;
  auto *q = new QSqlQuery(db);
  exec(q, todo::CreateMainTable);
  exec(q, todo::WithDraw, QStringList(_container.tableName));
  QStringList vs;
  vs.append(_container.tableName);
  vs.append(_container.tableTitle);
  vs.append(QString(_container.isReadOnly));
  vs.append(QString(_container.isPrivate));
  vs.append(QString(_container.isCatching));
  exec(q, todo::WriteOptions, vs);
  db.close();
  delete q;
}

/*!
 * Arguments: container {_container} [container properties],
 *            int {address} [address of expression],
 *            QString {expression},
 *            QString {links} [links to other expressions].
 * Inserts a new expression into the container.
 */
void sqlite::insert(const container &_container, int address,
                    const QString &expression, const QString &links) {
  QSqlDatabase db = prepare(_container.path);
  if (db.databaseName() == QString()) return;
  auto *q = new QSqlQuery(db);
  QStringList vs;
  vs.append(_container.tableName);
  vs.append(QString::number(address));
  vs.append(expression);
  vs.append(links);
  exec(q, todo::InsertExpression, vs);
  db.close();
  delete q;
}

/*!
 * Arguments: container {_container} [container properties],
 *            int {address} [address of expression].
 * Finds an expression and links by address.
 * Returns: QPair expression-links.
 */
QPair<QString, QString> sqlite::getExpression(const container &_container,
                                              int address) {
  QSqlDatabase db = prepare(_container.path);
  if (db.databaseName() == QString()) return QPair<QString, QString>();
  auto *q = new QSqlQuery(db);
  QStringList vs;
  vs.append(_container.tableName);
  vs.append(QString::number(address));
  exec(q, todo::SelectExpressionAndLinksByAddress, vs);
  db.close();
  q->first();
  QPair<QString, QString> enls;
  if (q->isValid()) {
    enls.first = q->value(0).toString();
    enls.second = q->value(1).toString();
  }
  delete q;
  return enls;
}

/*!
 * Arguments: container {_container} [container properties],
 *            QString {expression}.
 * Finds all activators in the expression.
 * Returns: QMap of activators-links.
 */
QMap<QString, QString> sqlite::scanContainer(const container &_container,
                                             const QString &expression) {
  QSqlDatabase db = prepare(_container.path);
  if (db.databaseName() == QString()) return QMap<QString, QString>();
  auto *q = new QSqlQuery(db);
  exec(q, todo::SelectExpressionsAndLinks, QStringList(_container.tableName));
  db.close();
  q->first();
  QMap<QString, QString> enls;
  while (q->isValid()) {
    if (expression.contains(handlers::purify(q->value(0).toString()),
                            Qt::CaseInsensitive))
      enls.insert(q->value(0).toString(), q->value(1).toString());
    q->next();
  }
  delete q;
  return enls;
}

/*!
 * Argument: container {_container} [container properties].
 * Indicates whether the container has additional properties.
 * Returns: true if has.
 */
bool sqlite::hasAdditionalProperties(const container &_container) {
  QSqlDatabase db = prepare(_container.path);
  if (db.databaseName() == QString()) return false;
  QSqlRecord r = db.record(_container.tableName);
  db.close();
  return (!r.fieldName(3).isNull());
}

/*!
 * Arguments: container {_container} [container properties],
 *            int {address} [address of expression].
 * Gets additional properties of the expression by {address}.
 * Returns: QMap of properties-values.
 */
QMap<QString, QString> sqlite::scanAdditionalProperties(
    const container &_container, int address) {
  QSqlDatabase db = prepare(_container.path);
  if (db.databaseName() == QString()) return QMap<QString, QString>();
  auto *q = new QSqlQuery(db);
  QStringList vs;
  vs.append(_container.tableName);
  vs.append(QString::number(address));
  exec(q, todo::SelectAdditionalProperties, vs);
  db.close();
  QSqlRecord r = q->record();
  delete q;
  int f = 3;
  QMap<QString, QString> aProps;
  while (!r.isNull(f)) {
    aProps.insert(r.fieldName(f), r.value(f).toString());
    f++;
  }
  return aProps;
}

/*!
 * Arguments: QString {path} [path to database],
 *            check {o} [options to check].
 * Prepares a database for work.
 * Returns: opened QSqlDatabase {db} with {path}.
 */
QSqlDatabase sqlite::prepare(const QString &path, check o) {
  if (o != check::OnlyOpen)
    if (!QFile::exists(path)) {
      emit sqliteError("Database \"" + path + "\" doesn't exist.");
      return QSqlDatabase();
    }
  QSqlDatabase db = QSqlDatabase::database();
  db.setDatabaseName(path);
  if (!db.open()) {
    emit sqliteError(db.lastError().text());
    db.setDatabaseName("");
    return db;
  }
  if ((o != check::OnlyOpen) && (o != check::AnyContent))
    if (db.tables().isEmpty()) {
      emit sqliteWarning("Database \"" + path + "\" is empty.");
      db.setDatabaseName("");
      return db;
    }
  return db;
}

/*!
 * Arguments: QSqlQuery {*q} [query],
 *            todo {o} [query type],
 *            QStringList {vs} [values for query].
 * Performs a database query.
 */
void sqlite::exec(QSqlQuery *q, todo o, QStringList vs) {
  // {*q} is passed to the function to return the result of the query by
  // reference.
  switch (o) {
    case todo::CreateMainTable:
      q->prepare(
          "CREATE TABLE IF NOT EXISTS containers (container TEXT, title TEXT, "
          "readOnly BOOL, private BOOL, catching BOOL)");
      break;
    case todo::CreateContainerTable:
      q->prepare(QString("CREATE TABLE IF NOT EXISTS \"%1\" (address INTEGER, "
                         "expression TEXT, "
                         "links TEXT)")
                     .arg(vs.at(0)));
      break;
    case todo::WithDraw:
      q->prepare("DELETE FROM containers WHERE container=:c");
      q->bindValue(":c", vs.at(0));
      break;
    case todo::LoadOptions:
      q->prepare(QString("SELECT title, readOnly, private, catching FROM "
                         "containers WHERE "
                         "container='%1'")
                     .arg(vs.at(0)));
      break;
    case todo::WriteOptions:
      q->prepare("INSERT INTO containers VALUES :c, :t, :ro, :pv, :ch");
      q->bindValue(":c", vs.at(0));
      q->bindValue(":t", vs.at(1));
      q->bindValue(":ro", vs.at(2).toInt());
      q->bindValue(":pv", vs.at(3).toInt());
      q->bindValue(":ch", vs.at(4).toInt());
      break;
    case todo::SelectContainers:
      q->prepare("SELECT * FROM containers");
      break;
    case todo::InsertExpression:
      q->prepare(QString("INSERT OR REPLACE INTO \"%1\" VALUES :a, :ex, :ls")
                     .arg(vs.at(0)));
      q->bindValue(":a", vs.at(1).toInt());
      q->bindValue(":ex", vs.at(2));
      q->bindValue(":ls", vs.at(3));
      break;
    case todo::SelectExpressionsAndLinks:
      q->prepare(QString("SELECT expression, links FROM \"%1\"").arg(vs.at(0)));
      break;
    case todo::SelectExpressionAndLinksByAddress:
      q->prepare(
          QString("SELECT expression, links FROM \"%1\" WHERE address = :a")
              .arg(vs.at(0)));
      q->bindValue(":a", vs.at(1).toInt());
      break;
    case todo::SelectAdditionalProperties:
      q->prepare(
          QString("SELECT * FROM \"%1\" WHERE address = :a").arg(vs.at(0)));
      q->bindValue(":a", vs.at(1).toInt());
      break;
  }
  if (!q->exec()) emit sqliteError(q->lastError().text() + " " + QString(o));
}
