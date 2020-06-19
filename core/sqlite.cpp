#include "sqlite.h"
#ifdef SQLITE_DEBUG
#include <QDebug>
#include <QElapsedTimer>
#endif

/*!
 * Argument: Source {source} [source properties].
 * Creates source.
 */
bool SQLite::create(const Source &source, QString *uuid) {
#ifdef SQLITE_AUTO_TESTS
  qDebug() << "\tDatabase path:" << _source.path;
#elif SQLITE_DEBUG
  qDebug() << "Database path:" << _source.path;
#endif
  // The container is created even if the database itself does not exist.
  QSqlDatabase db = prepare(source.path, check::OnlyOpen);
  if (db.databaseName().isEmpty()) {
#ifdef SQLITE_AUTO_TESTS
    qDebug() << "\tDatabase name is empty.";
#elif SQLITE_DEBUG
    qDebug() << "Database name is empty.";
#endif
    return false;
  }
  // Generates a table name. User-entered name may be incorrect for SQLite
  // database.
  QString uuid_to_verify;
  int cntr = 0;
  QStringList tables = db.tables();
  QRandomGenerator rand(quint32(QTime::currentTime().msec()));
  do {
    uuid_to_verify =
        QUuid(uint(rand.generate()), ushort(rand.generate()),
              ushort(rand.generate()), uchar(rand.bounded(0, 256)),
              uchar(rand.bounded(0, 256)), uchar(rand.bounded(0, 256)),
              uchar(rand.bounded(0, 256)), uchar(rand.bounded(0, 256)),
              uchar(rand.bounded(0, 256)), uchar(rand.bounded(0, 256)),
              uchar(rand.bounded(0, 256)))
            .toString();
    cntr++;
  } while ((tables.contains(uuid_to_verify)) &&
           (cntr < maximum_number_of_attempts));
  if (tables.contains(uuid_to_verify)) {
    emit sqliteError(
        "Could not create source, number of attempts exceeded " +
        QString(maximum_number_of_attempts) + ".");
#ifdef SQLITE_AUTO_TESTS
    qDebug() << "\tCould not create container, number of attempts exceeded " +
                    QString(maximum_number_of_attempts) + ".";
#elif SQLITE_DEBUG
    qDebug() << "Could not create container, number of attempts exceeded " +
                    QString(maximum_number_of_attempts) + ".";
#endif
    return false;
  }
  *uuid = uuid_to_verify;
  auto *query = new QSqlQuery(db);
  if (not exec(query, todo::CreateMainTable)) {
    emit sqliteError("Could not create main table.");
#ifdef SQLITE_AUTO_TESTS
    qDebug() << "\tCould not create main table.";
#elif SQLITE_DEBUG
    qDebug() << "Could not create main table.";
#endif
    return false;
  }
  QStringList vs;
  vs.append(*uuid);
  vs.append(source.table_title);
  vs.append(QString(source.is_read_only));
  vs.append(QString(source.is_private));
  vs.append(QString(source.is_catching));
  vs.append(QString(source.is_prioritised));
  if (not(exec(query, todo::WriteOptions, vs) and
          exec(query, todo::CreateSourceTable, QStringList(*uuid)))) {
    emit sqliteError("Could not write options and create source table.");
#ifdef SQLITE_AUTO_TESTS
    qDebug() << "\tCould not write options and create source table.";
#elif SQLITE_DEBUG
    qDebug() << "Could not write options and create source table.";
#endif
    return false;
  }
  db.close();
  delete query;
  return true;
}

/*!
 * Argument: QString {path} [path to database].
 * Searches for database sources of activators and reagents.
 * Returns: QList of sources {sources}.
 */
QList<Source> SQLite::sources(const QString &path) {
  QSqlDatabase db = prepare(path, check::AnyContent);
  if (db.databaseName() == QString())
    return QList<Source>();
  auto *query = new QSqlQuery(db);
  exec(query, todo::SelectSources);
  query->first();
  QList<Source> sources;
  while (query->isValid()) {
    Source source;
    source.path = path;
    source.table_name = query->value(0).toString();
    source.table_title = query->value(1).toString();
    source.is_read_only = query->value(2).toInt();
    source.is_private = query->value(3).toInt();
    source.is_catching = query->value(4).toInt();
    source.is_prioritised = query->value(5).toInt();
    sources.append(source);
    query->next();
  }
  db.close();
  delete query;
  return sources;
}

/*!
 * Argument: Source {source} [source properties].
 * Loads source properties from the database.
 * Returns: source properties {source}.
 */
Source SQLite::load(Source source) {
  QSqlDatabase db = prepare(source.path);
  if (db.databaseName() == QString())
    return source;
  auto *query = new QSqlQuery(db);
  exec(query, todo::LoadOptions, QStringList(source.table_name));
  db.close();
  if (not query->first())
    return source;
  source.table_title = query->value(0).toString();
  source.is_read_only = query->value(1).toBool();
  source.is_private = query->value(2).toBool();
  source.is_catching = query->value(3).toBool();
  source.is_prioritised = query->value(4).toBool();
  delete query;
  return source;
}

/*!
 * Argument: Source {source} [source properties].
 * Writes source properties into the database.
 */
void SQLite::write(const Source &source) {
  QSqlDatabase db = prepare(source.path, check::AnyContent);
  if (db.databaseName().isEmpty())
    return;
  auto *query = new QSqlQuery(db);
  exec(query, todo::CreateMainTable);
  exec(query, todo::WithDraw, QStringList(source.table_name));
  QStringList values;
  values.append(source.table_name);
  values.append(source.table_title);
  values.append(QString(source.is_read_only));
  values.append(QString(source.is_private));
  values.append(QString(source.is_catching));
  values.append(QString(source.is_prioritised));
  exec(query, todo::WriteOptions, values);
  db.close();
  delete query;
}

/*!
 * Arguments: Source {source} [source properties],
 *            int {address} [address of expression],
 *            QString {expression},
 *            QString {links} [links to other expressions].
 * Inserts a new expression into the source.
 */
void SQLite::insert(const Source &source, int address,
                    const QString &expression, const QString &links) {
  QSqlDatabase db = prepare(source.path);
  if (db.databaseName().isEmpty())
    return;
  auto *query = new QSqlQuery(db);
  QStringList values;
  values.append(source.table_name);
  values.append(QString::number(address));
  values.append(expression);
  values.append(links);
  exec(query, todo::InsertExpression, values);
  db.close();
  delete query;
}

/*!
 * Arguments: Source {source} [source properties],
 *            int {address} [address of expression].
 * Finds an expression and links by address.
 * Returns: QPair expression-links.
 */
QPair<QString, QString> SQLite::getExpression(const Source &source,
                                              int address) {
  QSqlDatabase db = prepare(source.path);
  if (db.databaseName().isEmpty())
    return QPair<QString, QString>();
  auto *query = new QSqlQuery(db);
  QStringList values;
  values.append(source.table_name);
  values.append(QString::number(address));
  exec(query, todo::SelectExpressionAndLinksByAddress, values);
  db.close();
  query->first();
  QPair<QString, QString> expression_links;
  if (query->isValid()) {
    expression_links.first = query->value(0).toString();
    expression_links.second = query->value(1).toString();
  }
  delete query;
  return expression_links;
}

/*!
 * Arguments: Source {source} [source properties],
 *            QString {expression}.
 * Finds all activators in the expression.
 * Returns: QMap of activators-links.
 */
QMap<QString, QString> SQLite::scanSource(const Source &source,
                                          const QString &expression) {
#ifdef SQLITE_SCANSOURCE_DEBUG
  QElapsedTimer timer;
  timer.start();
#endif
  QSqlDatabase db = prepare(source.path);
  if (db.databaseName().isEmpty())
    return QMap<QString, QString>();
  auto *query = new QSqlQuery(db);
  exec(query, todo::SelectExpressionsAndLinks, QStringList(source.table_name));
  query->first();
  QMap<QString, QString> expression_links;
  while (query->isValid()) {
    // If the expression includes a value from the table, then this value is an
    // activator.
    if (expression.contains(purify(query->value(0).toString())))
      expression_links.insert(query->value(0).toString(),
                              query->value(1).toString());
    query->next();
  }
  db.close();
  delete query;
#ifdef SQLITE_SCANSOURCE_DEBUG
  qDebug() << "SQLite::scanSource:" << timer.nsecsElapsed();
#endif
  return expression_links;
}

/*!
 * Argument: Source {source} [source properties].
 * Indicates whether the source has additional properties.
 * Returns: true if has.
 */
bool SQLite::hasAdditionalProperties(const Source &source) {
  QSqlDatabase db = prepare(source.path);
  if (db.databaseName().isEmpty())
    return false;
  QSqlRecord record = db.record(source.table_name);
  db.close();
  return (not record.fieldName(3).isNull());
}

/*!
 * Arguments: Source {source} [source properties],
 *            int {address} [address of expression].
 * Gets additional properties of the expression by {address}.
 * Returns: QMap of properties-values.
 */
QMap<QString, QString> SQLite::scanAdditionalProperties(const Source &source,
                                                        int address) {
  QSqlDatabase db = prepare(source.path);
  if (db.databaseName().isEmpty())
    return QMap<QString, QString>();
  auto *query = new QSqlQuery(db);
  QStringList values;
  values.append(source.table_name);
  values.append(QString::number(address));
  exec(query, todo::SelectAdditionalProperties, values);
  QSqlRecord record = query->record();
  db.close();
  delete query;
  QMap<QString, QString> additional_properties;
  for (int row = init_additionals_rows;
       not record.isNull(init_additionals_rows); row++) {
    additional_properties.insert(record.fieldName(row),
                                 record.value(row).toString());
  }
  return additional_properties;
}

/*!
 * Arguments: QString {path} [path to database],
 *            check {option} [options to check].
 * Prepares a database for work.
 * Returns: opened QSqlDatabase {db} with {path}.
 */
QSqlDatabase SQLite::prepare(const QString &path, check option) {
#ifdef SQLITE_AUTO_TESTS
  qDebug() << "\tSQLite::prepare: entered in.";
#endif
  if (option != check::OnlyOpen)
    if (not QFile::exists(path)) {
      emit sqliteError(tr("Database") + " \"" + path + "\" " +
                       tr("doesn't exist."));
#ifdef SQLITE_AUTO_TESTS
      qDebug() << "\tSQLite::prepare: error: Database doesn't exist.";
#endif
      return QSqlDatabase();
    }
  QSqlDatabase db = QSqlDatabase::database();
  db.setDatabaseName(path);
  if (not db.open()) {
    emit sqliteError(db.lastError().text());
#ifdef SQLITE_AUTO_TESTS
    qDebug() << "\tSQLite::prepare: error:" << db.lastError().text();
#endif
    db.setDatabaseName(QString());
    return db;
  }
  if ((option != check::OnlyOpen) && (option != check::AnyContent))
    if (db.tables().isEmpty()) {
      emit sqliteWarning(tr("Database") + " \"" + path + "\" " +
                         tr("is empty."));
#ifdef SQLITE_AUTO_TESTS
      qDebug() << "\tSQLite::prepare: error: Database is empty.";
#endif
      db.setDatabaseName(QString());
      return db;
    }
  return db;
}

/*!
 * Arguments: QSqlQuery {*query} [query],
 *            todo {option} [query type],
 *            QStringList {values} [values for query].
 * Performs a database query.
 */
bool SQLite::exec(QSqlQuery *query, todo option, QStringList values) {
  switch (option) {
  case todo::CreateMainTable:
    query->prepare(
        "CREATE TABLE IF NOT EXISTS sources (source TEXT NOT NULL UNIQUE, "
        "title TEXT, isReadOnly INTEGER NOT NULL, isPrivate INTEGER NOT NULL, "
        "isCatching INTEGER NOT NULL, isPrioritised INTEGER NOT NULL);");
    break;
  case todo::CreateSourceTable:
    query->prepare(
        QString(
            "CREATE TABLE IF NOT EXISTS \"%1\" (address INTEGER NOT NULL "
            "PRIMARY KEY AUTOINCREMENT UNIQUE, expression TEXT, links TEXT)")
            .arg(values[0]));
    break;
  case todo::WithDraw:
    query->prepare("DELETE FROM sources WHERE source=:c");
    query->bindValue(":c", values[0]);
    break;
  case todo::LoadOptions:
    query->prepare(QString("SELECT title, isReadOnly, isPrivate, isCatching, "
                           "isPrioritised FROM sources WHERE source = '%1'")
                       .arg(values[0]));
    break;
  case todo::WriteOptions:
    query->prepare("INSERT INTO sources VALUES (:c, :t, :ro, :pv, :ch, :pr)");
    query->bindValue(":c", values[0]);
    query->bindValue(":t", values[1]);
    query->bindValue(":ro", values[2].toInt());
    query->bindValue(":pv", values[3].toInt());
    query->bindValue(":ch", values[4].toInt());
    query->bindValue(":pr", values[5].toInt());
    break;
  case todo::SelectSources:
    query->prepare("SELECT * FROM sources");
    break;
  case todo::InsertExpression:
    query->prepare(
        QString("INSERT OR REPLACE INTO \"%1\" VALUES (:a, :ex, :ls)")
            .arg(values[0]));
    query->bindValue(":a", values[1].toInt());
    query->bindValue(":ex", values[2]);
    query->bindValue(":ls", values[3]);
    break;
  case todo::SelectExpressionsAndLinks:
    query->prepare(
        QString("SELECT expression, links FROM \"%1\"").arg(values[0]));
    break;
  case todo::SelectExpressionAndLinksByAddress:
    query->prepare(
        QString("SELECT expression, links FROM \"%1\" WHERE address = :a")
            .arg(values[0]));
    query->bindValue(":a", values[1].toInt());
    break;
  case todo::SelectAdditionalProperties:
    query->prepare(
        QString("SELECT * FROM \"%1\" WHERE address = :a").arg(values[0]));
    query->bindValue(":a", values[1].toInt());
    break;
  }
  if (not query->exec()) {
    emit sqliteError(query->lastError().text() + " " + QString(int(option)));
    return false;
  }
  return true;
}
