#include "sqlite.h"
#ifdef SQLITE_DEBUG
#include <QDebug>
#include <QElapsedTimer>
#endif

/*!
 * @fn SQLite::create_source
 * @brief Creates source.
 * @param[in] source source properties
 * @param[in,out] *uuid pointer to source's table name
 * @returns boolean value of source creation success
 */
bool SQLite::create_source(const Source &source, QString *uuid) {
  /*! Pieces of code for testing. */
#ifdef SQLITE_AUTO_TESTS
  qDebug() << "\tDatabase path:" << _source.path;
#elif SQLITE_DEBUG
  qDebug() << "Database path:" << _source.path;
#endif
  /*! The source is created even if the database itself does not exist. */
  bool *correct = new bool(true);
  QSqlDatabase db = prepare(source.path, Correct, correct);
  if (db.databaseName().isEmpty()) {
#ifdef SQLITE_AUTO_TESTS
    qDebug() << "\tDatabase name is empty.";
#elif SQLITE_DEBUG
    qDebug() << "Database name is empty.";
#endif
    return false;
  }
  auto *query = new QSqlQuery(db);
  if (not *correct) {
    if (not create_base_structure(query)) {
      emit sqliteError("Could not create main table.");
#ifdef SQLITE_AUTO_TESTS
      qDebug() << "\tCould not create main table.";
#elif SQLITE_DEBUG
      qDebug() << "Could not create main table.";
#endif
      delete query;
      return false;
    }
  }
  /*! Generates a table name. User-entered name may be incorrect for SQLite
   * database. */
  QString uuid_to_verify;
  int cntr = 0;
  QStringList tables = db.tables();
  QRandomGenerator rand(quint32(QTime::currentTime().msec()));
  do {
    /*! Generates UUID. */
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
           (cntr < maximum_number_of_attempts)); /*!< Generation is repeated
                                                  * until a unique UUID is
                                                  * found. */
  if (tables.contains(uuid_to_verify)) { /*!< If the generation is unsuccessful,
                                          * it throws an error. */
    emit sqliteError("Could not create source, number of attempts exceeded " +
                     QString(maximum_number_of_attempts) + ".");
#ifdef SQLITE_AUTO_TESTS
    qDebug() << "\tCould not create container, number of attempts exceeded " +
                    QString(maximum_number_of_attempts) + ".";
#elif SQLITE_DEBUG
    qDebug() << "Could not create container, number of attempts exceeded " +
                    QString(maximum_number_of_attempts) + ".";
#endif
    delete query;
    return false;
  }
  *uuid = uuid_to_verify; /*!< The UUID is passed by pointer to the object that
                           * called this method.
                           * @details This may be necessary for those objects
                           * that want to continue editing a new table. */
  /*! The following queries write the parameters about the table being created
   * (the source with the @a source parameters) to the main table and create
   * the source table with the name generated in the form of UUID @a *uuid. */
  if (not(exec(query, WriteOptions,
               {*uuid, source.table_title, QString(source.is_read_only),
                QString(source.is_private), QString(source.is_catching),
                QString(source.is_prioritised)}) and
          exec(query, CreateSourceTable, QStringList(*uuid)))) {
    emit sqliteError("Could not write options and create source table.");
#ifdef SQLITE_AUTO_TESTS
    qDebug() << "\tCould not write options and create source table.";
#elif SQLITE_DEBUG
    qDebug() << "Could not write options and create source table.";
#endif
    delete query;
    return false;
  }
  db.close();
  delete query;
  return true;
}

bool SQLite::create_base_structure(QSqlQuery *query) {
  bool result = true;
  /*! The following queries create a source table if it does not exist, or
   * recreate it if it is incorrect. */
  result &= exec(query, RemoveMainTableIfExists);
  result &= exec(query, CreateMainTableIfNotExists);
  return result;
}

/*!
 * @fn SQLite::sources
 * @brief Searches for database sources of activators and reagents.
 * @param[in] path path to database
 * @returns list of sources @a sources
 */
QList<Source> SQLite::sources(const QString &path) {
  QSqlDatabase db = prepare(path, Openable);
  if (db.databaseName().isEmpty())
    return QList<Source>();
  auto *query = new QSqlQuery(db);
  exec(query, SelectSources);
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
 * @fn SQLite::load_source
 * @brief Loads source properties from the database.
 * @param[in] source incomplete source data (database and table name)
 * @returns complete source properties
 */
Source SQLite::load_source(Source source) {
  QSqlDatabase db = prepare(source.path);
  if (db.databaseName().isEmpty())
    return source;
  auto *query = new QSqlQuery(db);
  exec(query, LoadOptions, QStringList(source.table_name));
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
 * @fn SQLite::write_source
 * @brief Writes source properties into the database.
 * @param[in] source source properties
 */
bool SQLite::write_source(const Source &source) {
  QSqlDatabase db = prepare(source.path, Openable);
  if (db.databaseName().isEmpty())
    return false;
  auto *query = new QSqlQuery(db);
  exec(query, CreateMainTableIfNotExists);
  exec(query, WithDraw, QStringList(source.table_name));
  bool result =
      exec(query, WriteOptions,
           {source.table_name, source.table_title, QString(source.is_read_only),
            QString(source.is_private), QString(source.is_catching),
            QString(source.is_prioritised)});
  db.close();
  delete query;
  return result;
}

/*!
 * @fn SQLite::insert_expression
 * @brief Inserts a new expression into the source.
 * @param[in] source source properties
 * @param[in] address address of expression
 * @param[in] expression expression
 * @param[in] links links to other expressions
 */
bool SQLite::insert_expression(const Source &source, int address,
                               const QString &expression,
                               const QString &links) {
  QSqlDatabase db = prepare(source.path);
  if (db.databaseName().isEmpty())
    return false;
  auto *query = new QSqlQuery(db);
  bool result =
      exec(query, InsertExpression,
           {source.table_name, QString::number(address), expression, links});
  db.close();
  delete query;
  return result;
}

/*!
 * @fn SQLite::get_expression
 * @brief Finds an expression and links by address.
 * @param[in] source source properties
 * @param[in] address address of expression
 * @returns expression-links pair
 */
QPair<QString, QString> SQLite::get_expression(const Source &source,
                                               int address) {
  QSqlDatabase db = prepare(source.path);
  if (db.databaseName().isEmpty())
    return QPair<QString, QString>();
  auto *query = new QSqlQuery(db);
  exec(query, SelectExpressionAndLinksByAddress,
       {source.table_name, QString::number(address)});
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
 * @fn SQLite::scan_source
 * @brief Finds all activators for the expression.
 * @param[in] source source properties
 * @param[in] expression expression
 * @returns activator-links map
 */
QMap<QString, QString> SQLite::scan_source(const Source &source,
                                           const QString &expression) {
#ifdef SQLITE_SCANSOURCE_DEBUG
  QElapsedTimer timer;
  timer.start();
#endif
  QSqlDatabase db = prepare(source.path);
  if (db.databaseName().isEmpty())
    return QMap<QString, QString>();
  auto *query = new QSqlQuery(db);
  exec(query, SelectExpressionsAndLinks, {source.table_name});
  query->first();
  QMap<QString, QString> expression_links;
  while (query->isValid()) {
    /*! If the expression includes a value from the table, then this value is an
     * activator. */
    if (expression.contains(purify(query->value(0).toString())))
      expression_links.insert(query->value(0).toString(),
                              query->value(1).toString());
    query->next();
  }
  db.close();
  delete query;
#ifdef SQLITE_SCANSOURCE_DEBUG
  qDebug() << "SQLite::scan_source:" << timer.nsecsElapsed();
#endif
  return expression_links;
}

/*!
 * @fn SQLite::has_additional_properties
 * @brief Indicates whether the source has additional properties.
 * @param[in] source source properties
 * @returns boolean value of the presence of additional properties at the
 * source.
 */
bool SQLite::has_additional_properties(const Source &source) {
  QSqlDatabase db = prepare(source.path);
  if (db.databaseName().isEmpty())
    return false;
  QSqlRecord record = db.record(source.table_name);
  db.close();
  return (not record.fieldName(3).isNull());
}

/*!
 * @fn SQLite::scan_additional_properties
 * @brief Gets additional properties of the expression by @a address.
 * @param[in] source source properties
 * @param[in] address address of expression
 * @returns key-value map of properties
 */
QMap<QString, QString> SQLite::scan_additional_properties(const Source &source,
                                                          int address) {
  QSqlDatabase db = prepare(source.path);
  if (db.databaseName().isEmpty())
    return QMap<QString, QString>();
  auto *query = new QSqlQuery(db);
  exec(query, SelectAdditionalProperties,
       {source.table_name, QString::number(address)});
  QSqlRecord record = query->record();
  db.close();
  delete query;
  QMap<QString, QString> additional_properties;
  for (int row = init_additionals_rows; not record.isNull(row); row++) {
    additional_properties.insert(record.fieldName(row),
                                 record.value(row).toString());
  }
  return additional_properties;
}

/*!
 * @fn SQLite::prepare
 * @brief Prepares a database for work.
 * @param[in] path path to database
 * @param[in] option option to check database
 * @param[in,out] result pointer to a variable that should contain the result of
 * the check
 * @param[in] quiet tells whether to display error messages
 * @returns opened QSqlDatabase @a db with @a path
 * @sa Check
 */
QSqlDatabase SQLite::prepare(const QString &path, Check option, bool *result,
                             bool quiet) {
  QSqlDatabase db = QSqlDatabase::database();
  switch (option) {
  case NoCheck: {
    db.setDatabaseName(path);
    db.open();
    break;
  }
  case Exists: {
    if (result)
      *result = true;
    if (not QFile::exists(path)) {
      if (not quiet) {
        emit sqliteError(tr("Database \"%1\" doesn't exist.").arg(path));
#ifdef SQLITE_AUTO_TESTS
        qDebug() << "\tSQLite::prepare: error: Database doesn't exist.";
#endif
      }
      if (result)
        *result = false;
      break;
    }
    db.setDatabaseName(path);
    db.open();
    break;
  }
  case Openable: {
    if (result)
      *result = true;
    db.setDatabaseName(path);
    if (not db.open()) {
      if (not quiet) {
        emit sqliteError(tr("Opening error. Error text: ") +
                         db.lastError().text());
#ifdef SQLITE_AUTO_TESTS
        qDebug() << "\tSQLite::prepare: opening error:"
                 << db.lastError().text();
#endif
      }
      if (result)
        *result = false;
    }
    break;
  }
  case Correct: {
    /*! Correct = Exists + Openable + has the correct structure. */
    if (result)
      *result = true;
    if (not QFile::exists(path)) {
      if (not quiet) {
        emit sqliteError(tr("Database \"%1\" doesn't exist.").arg(path));
#ifdef SQLITE_AUTO_TESTS
        qDebug() << "\tSQLite::prepare: error: Database doesn't exist.";
#endif
      }
      if (result)
        *result = false;
      break;
    }
    db.setDatabaseName(path);
    if (not db.open()) {
      if (not quiet) {
        emit sqliteError(tr("Opening error. Error text: ") +
                         db.lastError().text());
#ifdef SQLITE_AUTO_TESTS
        qDebug() << "\tSQLite::prepare: opening error:"
                 << db.lastError().text();
#endif
      }
      if (result)
        *result = false;
    }
    bool valid = validate(&db, false, quiet);
    if (result)
      *result = valid;
    break;
  }
  case RecursivelyСorrect: {
    if (result)
      *result = true;
    if (not QFile::exists(path)) {
      if (not quiet) {
        emit sqliteError(tr("Database \"%1\" doesn't exist.").arg(path));
#ifdef SQLITE_AUTO_TESTS
        qDebug() << "\tSQLite::prepare: error: Database doesn't exist.";
#endif
      }
      if (result)
        *result = false;
      break;
    }
    db.setDatabaseName(path);
    if (not db.open()) {
      if (not quiet) {
        emit sqliteError(tr("Opening error. Error text: ") +
                         db.lastError().text());
#ifdef SQLITE_AUTO_TESTS
        qDebug() << "\tSQLite::prepare: opening error:"
                 << db.lastError().text();
#endif
      }
      if (result)
        *result = false;
    }
    bool valid = validate(&db, true, quiet);
    if (result)
      *result = valid;
    break;
  }
  }
  return db;
}

/*!
 * @fn SQLite::validate
 * @brief Validates the database.
 * @param[in,out] db reference to the database
 * @param[in] recursive tells whether to check all containers in the database
 * @param[in] quiet tells whether to display error messages
 * @returns result of validation
 * @retval true validation successful, no errors found
 * @retval false error was encountered, the database is invalid
 */
bool SQLite::validate(QSqlDatabase *db, bool recursive, bool quiet) {
  QString db_suffix = tr("Database \"%1\".").arg(db->databaseName());
  auto query = QSqlQuery(*db);
  exec(&query, IfMainTableExists);
  query.first();
  if (not query.isValid() and not quiet) {
    emit sqliteError(
        tr("Validation error: table with sources does not exist.") + " " +
        db_suffix);
    return false;
  }
  exec(&query, IfMainTableCorrect);
  if (not query.first() and not quiet) {
    emit sqliteError(tr("Validation error: table with sources is empty.") +
                     " " + db_suffix);
    return false;
  }
  bool sourceColumnValid = true;
  sourceColumnValid &= query.value(1).toString() == "source";
  sourceColumnValid &= query.value(2).toString() == "TEXT";
  sourceColumnValid &= query.value(3).toInt() == 1;
  if (not sourceColumnValid and not quiet) {
    emit sqliteError(
        tr("Validation error: the first column of the table with sources does "
           "not fit the description of \"source\" TEXT NOT NULL UNIQUE.") +
        " " + db_suffix);
    return false;
  }
  if (not query.next() and not quiet) {
    emit sqliteError(tr("Validation error: the table with sources contains "
                        "only one column.") +
                     " " + db_suffix);
    return false;
  }
  bool titleColumnValid = true;
  titleColumnValid &= query.value(1).toString() == "title";
  titleColumnValid &= query.value(2).toString() == "TEXT";
  if (not titleColumnValid and not quiet) {
    emit sqliteError(
        tr("Validation error: the second column of the table with sources does "
           "not fit the description of \"title\" TEXT.") +
        " " + db_suffix);
    return false;
  }
  if (not query.next() and not quiet) {
    emit sqliteError(tr("Validation error: the table with sources contains "
                        "only two columns.") +
                     " " + db_suffix);
    return false;
  }
  bool isReadOnlyColumnValid = true;
  isReadOnlyColumnValid &= query.value(1).toString() == "isReadOnly";
  isReadOnlyColumnValid &= query.value(2).toString() == "INTEGER";
  isReadOnlyColumnValid &= query.value(3).toInt() == 1;
  if (not isReadOnlyColumnValid and not quiet) {
    emit sqliteError(
        tr("Validation error: the third column of the table with sources does "
           "not fit the description of \"isReadOnly\" INTEGER NOT NULL.") +
        " " + db_suffix);
    return false;
  }
  if (not query.next() and not quiet) {
    emit sqliteError(tr("Validation error: the table with sources contains "
                        "only three columns.") +
                     " " + db_suffix);
    return false;
  }
  bool isPrivateColumnValid = true;
  isPrivateColumnValid &= query.value(1).toString() == "isPrivate";
  isPrivateColumnValid &= query.value(2).toString() == "INTEGER";
  isPrivateColumnValid &= query.value(3).toInt() == 1;
  if (not isPrivateColumnValid and not quiet) {
    emit sqliteError(
        tr("Validation error: the fourth column of the table with sources does "
           "not fit the description of \"isPrivate\" INTEGER NOT NULL.") +
        " " + db_suffix);
    return false;
  }
  if (not query.next() and not quiet) {
    emit sqliteError(tr("Validation error: the table with sources contains "
                        "only four columns.") +
                     " " + db_suffix);
    return false;
  }
  bool isCatchingColumnValid = true;
  isCatchingColumnValid &= query.value(1).toString() == "isCatching";
  isCatchingColumnValid &= query.value(2).toString() == "INTEGER";
  isCatchingColumnValid &= query.value(3).toInt() == 1;
  if (not isCatchingColumnValid and not quiet) {
    emit sqliteError(
        tr("Validation error: the fifth column of the table with sources does "
           "not fit the description of \"isCatching\" INTEGER NOT NULL.") +
        " " + db_suffix);
    return false;
  }
  if (not query.next() and not quiet) {
    emit sqliteError(tr("Validation error: the table with sources contains "
                        "only five columns.") +
                     " " + db_suffix);
    return false;
  }
  bool isPrioritisedColumnValid = true;
  isPrioritisedColumnValid &= query.value(1).toString() == "isPrioritised";
  isPrioritisedColumnValid &= query.value(2).toString() == "INTEGER";
  isPrioritisedColumnValid &= query.value(3).toInt() == 1;
  if (not isPrioritisedColumnValid and not quiet) {
    emit sqliteError(
        tr("Validation error: the sixth column of the table with sources does "
           "not fit the description of \"isPrioritised\" INTEGER NOT NULL.") +
        " " + db_suffix);
    return false;
  }
  if (recursive) {
    exec(&query, SelectSources);
    if (not query.first())
      return true;
    bool eachSourceValid = true;
    do
      eachSourceValid &= validate(db, query.value(0).toString());
    while (query.next());
    return eachSourceValid;
  }
  return true;
}

/*!
 * @fn SQLite::validate
 * @brief Validates the database source.
 * @param[in,out] db reference to the database
 * @param[in] source_table source name
 * @param[in] quiet tells whether to display error messages
 * @returns result of validation
 * @retval true validation successful, no errors found
 * @retval false error was encountered, the source is invalid
 */
bool SQLite::validate(QSqlDatabase *db, const QString &source_table,
                      bool quiet) {
  QString db_source_suffix = tr("Database \"%1\", source \"%2\".")
                                 .arg(db->databaseName())
                                 .arg(source_table);
  auto query = QSqlQuery(*db);
  exec(&query, IfSourceTableExists, {source_table});
  if (not query.isValid() and not quiet) {
    emit sqliteError(tr("Validation error: source does not exist.") + " " +
                     db_source_suffix);
    return false;
  }
  exec(&query, IfSourceTableCorrect, {source_table});
  if (not query.first() and not quiet) {
    emit sqliteError(tr("Validation error: source is empty.") + " " +
                     db_source_suffix);
    return false;
  }
  bool addressColumnValid = true;
  addressColumnValid &= query.value(1).toString() == "address";
  addressColumnValid &= query.value(2).toString() == "INTEGER";
  addressColumnValid &= query.value(3).toInt() == 1;
  addressColumnValid &= query.value(5).toInt() == 1;
  if (not addressColumnValid and not quiet) {
    emit sqliteError(tr("Validation error: the first column of the source does "
                        "not fit the description of \"address\" INTEGER NOT "
                        "NULL PRIMARY KEY AUTOINCREMENT UNIQUE.") +
                     " " + db_source_suffix);
    return false;
  }
  if (not query.next() and not quiet) {
    emit sqliteError(
        tr("Validation error: the source contains only one column.") + " " +
        db_source_suffix);
    return false;
  }
  bool expressionColumnValid = true;
  expressionColumnValid &= query.value(1).toString() == "expression";
  expressionColumnValid &= query.value(2).toString() == "TEXT";
  if (not expressionColumnValid and not quiet) {
    emit sqliteError(
        tr("Validation error: the second column of the source does not fit the "
           "description of \"expression\" TEXT.") +
        " " + db_source_suffix);
    return false;
  }
  if (not query.next() and not quiet) {
    emit sqliteError(
        tr("Validation error: the source contains only two columns.") + " " +
        db_source_suffix);
    return false;
  }
  bool linksColumnValid = true;
  linksColumnValid &= query.value(1).toString() == "links";
  linksColumnValid &= query.value(2).toString() == "TEXT";
  if (not linksColumnValid and not quiet) {
    emit sqliteError(tr("Validation error: the third column of the source "
                        "does not fit the description of \"links\" TEXT.") +
                     " " + db_source_suffix);
    return false;
  }
  return true;
}

/*!
 * Arguments: QSqlQuery {*query} [query],
 *            todo {option} [query type],
 *            QStringList {values} [values for query].
 * Performs a database query.
 */
/*!
 * @fn SQLite::exec
 * @brief Performs a database query.
 * @param[in,out] query SQL query
 * @param[in] option query type
 * @param[in] values values for query
 * @returns SQL query result
 * @retval true SQL query executed successfully
 * @retval false SQL query was not executed correctly
 */
bool SQLite::exec(QSqlQuery *query, ToDo option, QStringList values) {
  switch (option) {
  case CreateMainTableIfNotExists:
    query->prepare(
        "CREATE TABLE IF NOT EXISTS sources (source TEXT NOT NULL UNIQUE, "
        "title TEXT, isReadOnly INTEGER NOT NULL, isPrivate INTEGER NOT NULL, "
        "isCatching INTEGER NOT NULL, isPrioritised INTEGER NOT NULL);");
    break;
  case CreateSourceTable:
    query->prepare(
        QString("CREATE TABLE \"%1\" (address INTEGER NOT NULL PRIMARY KEY "
                "AUTOINCREMENT UNIQUE, expression TEXT, links TEXT)")
            .arg(values[0]));
    break;
  case WithDraw:
    query->prepare("DELETE FROM sources WHERE source=:c");
    query->bindValue(":c", values[0]);
    break;
  case LoadOptions:
    query->prepare(QString("SELECT title, isReadOnly, isPrivate, isCatching, "
                           "isPrioritised FROM sources WHERE source = '%1'")
                       .arg(values[0]));
    break;
  case WriteOptions:
    query->prepare("INSERT INTO sources VALUES (:c, :t, :ro, :pv, :ch, :pr)");
    query->bindValue(":c", values[0]);
    query->bindValue(":t", values[1]);
    query->bindValue(":ro", values[2].toInt());
    query->bindValue(":pv", values[3].toInt());
    query->bindValue(":ch", values[4].toInt());
    query->bindValue(":pr", values[5].toInt());
    break;
  case SelectSources:
    query->prepare("SELECT * FROM sources");
    break;
  case InsertExpression:
    query->prepare(
        QString("INSERT OR REPLACE INTO \"%1\" VALUES (:a, :ex, :ls)")
            .arg(values[0]));
    query->bindValue(":a", values[1].toInt());
    query->bindValue(":ex", values[2]);
    query->bindValue(":ls", values[3]);
    break;
  case SelectExpressionsAndLinks:
    query->prepare(
        QString("SELECT expression, links FROM \"%1\"").arg(values[0]));
    break;
  case SelectExpressionAndLinksByAddress:
    query->prepare(
        QString("SELECT expression, links FROM \"%1\" WHERE address = :a")
            .arg(values[0]));
    query->bindValue(":a", values[1].toInt());
    break;
  case SelectAdditionalProperties:
    query->prepare(
        QString("SELECT * FROM \"%1\" WHERE address = :a").arg(values[0]));
    query->bindValue(":a", values[1].toInt());
    break;
  case IfMainTableExists:
    query->prepare("SELECT name FROM sqlite_master WHERE type='table' AND "
                   "name='sources';");
    break;
  case IfMainTableCorrect:
    query->prepare("PRAGMA table_info('sources');");
    break;
  case IfSourceTableExists:
    query->prepare("SELECT name FROM sqlite_master WHERE type='table' AND "
                   "name=':n';");
    query->bindValue(":n", values[0]);
    break;
  case IfSourceTableCorrect:
    query->prepare("PRAGMA table_info(':n');");
    query->bindValue(":n", values[0]);
    break;
  case RemoveMainTableIfExists:
    query->prepare("DROP TABLE IF EXISTS 'sources';");
    break;
  case RemoveSourceTableIfExists:
    query->prepare("DROP TABLE IF EXISTS ':n';");
    query->bindValue(":n", values[0]);
    break;
  case NoneToDo:;
  }
  if (not query->exec()) {
    emit sqliteError(query->lastError().text() + " " + QString(int(option)));
    return false;
  }
  return true;
}
