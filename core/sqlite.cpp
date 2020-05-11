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
 * Argument: Source {_source} [source properties].
 * Creates container.
 */
void SQLite::create(const Source &_source) {
  // The container is created even if the database itself does not exist.
  QSqlDatabase db = prepare(_source.path, check::OnlyOpen);
  if (db.databaseName() == QString())
    return;
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
  vs.append(_source.tableTitle);
  vs.append(QString(_source.isReadOnly));
  vs.append(QString(_source.isPrivate));
  vs.append(QString(_source.isCatching));
  vs.append(QString(_source.isPrioritised));
  exec(q, todo::WriteOptions, vs);
  exec(q, todo::CreateContainerTable, QStringList(uuid));
  db.close();
  delete q;
}

/*!
 * Argument: QString {path} [path to database].
 * Searches for database sources of activators and reagents.
 * Returns: QList of sources {cProps}.
 */
QList<Source> SQLite::sources(const QString &path) {
  QSqlDatabase db = prepare(path, check::AnyContent);
  if (db.databaseName() == QString())
    return QList<Source>();
  auto *q = new QSqlQuery(db);
  exec(q, todo::SelectContainers);
  q->first();
  QList<Source> cProps;
  while (q->isValid()) {
    Source cProp;
    cProp.path = path;
    cProp.tableName = q->value(0).toString();
    cProp.tableTitle = q->value(1).toString();
    cProp.isReadOnly = q->value(2).toInt();
    cProp.isPrivate = q->value(3).toInt();
    cProp.isCatching = q->value(4).toInt();
    cProp.isPrioritised = q->value(5).toInt();
    cProps.append(cProp);
    q->next();
  }
  db.close();
  delete q;
  return cProps;
}

/*!
 * Argument: Source {_source} [source properties].
 * Loads source properties from the database.
 * Returns: source properties {_source}.
 */
Source SQLite::load(Source _source) {
  QSqlDatabase db = prepare(_source.path);
  if (db.databaseName() == QString())
    return _source;
  auto *q = new QSqlQuery(db);
  exec(q, todo::LoadOptions, QStringList(_source.tableName));
  db.close();
  if (!q->first())
    return _source;
  _source.tableTitle = q->value(0).toString();
  _source.isReadOnly = q->value(1).toBool();
  _source.isPrivate = q->value(2).toBool();
  _source.isCatching = q->value(3).toBool();
  _source.isPrioritised = q->value(4).toBool();
  delete q;
  return _source;
}

/*!
 * Argument: Source {_source} [source properties].
 * Writes source properties into the database.
 */
void SQLite::write(const Source &_source) {
  QSqlDatabase db = prepare(_source.path, check::AnyContent);
  if (db.databaseName() == QString())
    return;
  auto *q = new QSqlQuery(db);
  exec(q, todo::CreateMainTable);
  exec(q, todo::WithDraw, QStringList(_source.tableName));
  QStringList vs;
  vs.append(_source.tableName);
  vs.append(_source.tableTitle);
  vs.append(QString(_source.isReadOnly));
  vs.append(QString(_source.isPrivate));
  vs.append(QString(_source.isCatching));
  vs.append(QString(_source.isPrioritised));
  exec(q, todo::WriteOptions, vs);
  db.close();
  delete q;
}

/*!
 * Arguments: Source {_source} [source properties],
 *            int {address} [address of expression],
 *            QString {expression},
 *            QString {links} [links to other expressions].
 * Inserts a new expression into the source.
 */
void SQLite::insert(const Source &_source, int address,
                    const QString &expression, const QString &links) {
  QSqlDatabase db = prepare(_source.path);
  if (db.databaseName() == QString())
    return;
  auto *q = new QSqlQuery(db);
  QStringList vs;
  vs.append(_source.tableName);
  vs.append(QString::number(address));
  vs.append(expression);
  vs.append(links);
  exec(q, todo::InsertExpression, vs);
  db.close();
  delete q;
}

/*!
 * Arguments: Source {_source} [source properties],
 *            int {address} [address of expression].
 * Finds an expression and links by address.
 * Returns: QPair expression-links.
 */
QPair<QString, QString> SQLite::getExpression(const Source &_source,
                                              int address) {
  QSqlDatabase db = prepare(_source.path);
  if (db.databaseName() == QString())
    return QPair<QString, QString>();
  auto *q = new QSqlQuery(db);
  QStringList vs;
  vs.append(_source.tableName);
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
 * Arguments: Source {_source} [source properties],
 *            QString {expression}.
 * Finds all activators in the expression.
 * Returns: QMap of activators-links.
 */
QMap<QString, QString> SQLite::scanSource(const Source &_source,
                                          const QString &expression) {
  QSqlDatabase db = prepare(_source.path);
  if (db.databaseName() == QString())
    return QMap<QString, QString>();
  auto *q = new QSqlQuery(db);
  exec(q, todo::SelectExpressionsAndLinks, QStringList(_source.tableName));
  q->first();
  QMap<QString, QString> enls;
  while (q->isValid()) {
    // If the expression includes a value from the table, then this value is an
    // activator.
    if (expression.contains(purify(q->value(0).toString())))
      enls.insert(q->value(0).toString(), q->value(1).toString());
    q->next();
  }
  db.close();
  delete q;
  return enls;
}

/*!
 * Argument: Source {_source} [Source properties].
 * Indicates whether the source has additional properties.
 * Returns: true if has.
 */
bool SQLite::hasAdditionalProperties(const Source &_source) {
  QSqlDatabase db = prepare(_source.path);
  if (db.databaseName() == QString())
    return false;
  QSqlRecord r = db.record(_source.tableName);
  db.close();
  return (!r.fieldName(3).isNull());
}

/*!
 * Arguments: Source {_source} [source properties],
 *            int {address} [address of expression].
 * Gets additional properties of the expression by {address}.
 * Returns: QMap of properties-values.
 */
QMap<QString, QString>
SQLite::scanAdditionalProperties(const Source &_source, int address) {
  QSqlDatabase db = prepare(_source.path);
  if (db.databaseName() == QString())
    return QMap<QString, QString>();
  auto *q = new QSqlQuery(db);
  QStringList vs;
  vs.append(_source.tableName);
  vs.append(QString::number(address));
  exec(q, todo::SelectAdditionalProperties, vs);
  QSqlRecord r = q->record();
  db.close();
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
QSqlDatabase SQLite::prepare(const QString &path, check o) {
  if (o != check::OnlyOpen)
    if (!QFile::exists(path)) {
      emit sqliteError(tr("Database") + " \"" + path + "\" " +
                       tr("doesn't exist."));
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
      emit sqliteWarning(tr("Database") + " \"" + path + "\" " +
                         tr("is empty."));
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
void SQLite::exec(QSqlQuery *q, todo o, QStringList vs) {
  // {*q} is passed to the function to return the result of the query by
  // reference.
  switch (o) {
  case todo::CreateMainTable:
    q->prepare(
        "CREATE TABLE IF NOT EXISTS sources (source TEXT NOT NULL UNIQUE, "
        "title TEXT, isReadOnly INTEGER NOT NULL, isPrivate INTEGER NOT NULL, "
        "isCatching INTEGER NOT NULL, isPrioritised INTEGER NOT NULL);");
    break;
  case todo::CreateContainerTable:
    q->prepare(
        QString(
            "CREATE TABLE IF NOT EXISTS \"%1\" (address INTEGER NOT NULL "
            "PRIMARY KEY AUTOINCREMENT UNIQUE, expression TEXT, links TEXT)")
            .arg(vs.at(0)));
    break;
  case todo::WithDraw:
    q->prepare("DELETE FROM sources WHERE source=:c");
    q->bindValue(":c", vs.at(0));
    break;
  case todo::LoadOptions:
    q->prepare(QString("SELECT title, isReadOnly, isPrivate, isCatching, "
                       "isPrioritised FROM sources WHERE source = '%1'")
                   .arg(vs.at(0)));
    break;
  case todo::WriteOptions:
    q->prepare("INSERT INTO sources VALUES (:c, :t, :ro, :pv, :ch, :pr)");
    q->bindValue(":c", vs.at(0));
    q->bindValue(":t", vs.at(1));
    q->bindValue(":ro", vs.at(2).toInt());
    q->bindValue(":pv", vs.at(3).toInt());
    q->bindValue(":ch", vs.at(4).toInt());
    q->bindValue(":pr", vs.at(5).toInt());
    break;
  case todo::SelectContainers:
    q->prepare("SELECT * FROM sources");
    break;
  case todo::InsertExpression:
    q->prepare(QString("INSERT OR REPLACE INTO \"%1\" VALUES (:a, :ex, :ls)")
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
  if (!q->exec())
    emit sqliteError(q->lastError().text() + " " + QString((int)o));
}
