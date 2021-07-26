#include "sqlite.h"
#include <iostream>
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
  qDebug() << "\tDatabase path:" << source.path;
#elif SQLITE_DEBUG
  qDebug() << "Database path:" << source.path;
#endif
  /*! The source is created even if the database itself does not exist. */
  auto *correct = new bool(true);
  auto db = prepare(source.path, Correct, correct);
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
      emit sqlite_error("Could not create main table.");
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
  auto tables = db.tables();
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
    emit sqlite_error("Could not create source, number of attempts exceeded " +
                      QString::number(maximum_number_of_attempts) + ".");
#ifdef SQLITE_AUTO_TESTS
    qDebug() << "\tCould not create container, number of attempts exceeded " +
                    QString::number(maximum_number_of_attempts) + ".";
#elif SQLITE_DEBUG
    qDebug() << "Could not create container, number of attempts exceeded " +
                    QString::number(maximum_number_of_attempts) + ".";
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
               {*uuid, source.table_title, QString::number(source.is_read_only),
                QString::number(source.is_private), QString::number(source.is_catching),
                QString::number(source.is_prioritised)}) and
          exec(query, CreateSourceTable, QStringList(*uuid)))) {
    emit sqlite_error("Could not write options and create source table.");
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

/*!
 * @fn SQLite::create_base_structure
 * @brief The following queries create a source table if it does not exist, or
 * recreate it if it is incorrect.
 * @param query reference to the QSqlQuery instance
 * @returns
 */
bool SQLite::create_base_structure(QSqlQuery *query) {
  bool result = true;
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
  auto db = prepare(path, Openable);
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
  auto db = prepare(source.path);
  if (db.databaseName().isEmpty())
    return source;
  auto *query = new QSqlQuery(db);
  exec(query, LoadOptions, {source.table_name});
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
  auto db = prepare(source.path, Openable);
  if (db.databaseName().isEmpty())
    return false;
  auto *query = new QSqlQuery(db);
  exec(query, CreateMainTableIfNotExists);
  exec(query, WithDraw, QStringList(source.table_name));
  auto result =
      exec(query, WriteOptions,
           {source.table_name, source.table_title, QString::number(source.is_read_only),
            QString::number(source.is_private), QString::number(source.is_catching),
            QString::number(source.is_prioritised)});
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
  auto db = prepare(source.path);
  if (db.databaseName().isEmpty())
    return false;
  auto *query = new QSqlQuery(db);
  auto result =
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
Expression SQLite::get_expression_by_address(const Source &source,
                                             int address) {
  auto db = prepare(source.path);
  if (db.databaseName().isEmpty())
    return Expression();
  auto *query = new QSqlQuery(db);
  exec(query, SelectELByAddress, {source.table_name, QString::number(address)});
  db.close();
  query->first();
  Expression expr;
  if (query->isValid()) {
    expr.activator_text = query->value(0).toString();
    expr.reagent_text = query->value(1).toString();
  }
  delete query;
  return expr;
}

/*!
 * @fn SQLite::scan_source
 * @brief Finds all activators for the expression.
 * @param[in] source source properties
 * @param[in] expression expression
 * @returns selection for given input
 */
LinkedCache SQLite::scan_source(const Source &source, const QString &input) {
#ifdef SQLITE_SCANSOURCE_DEBUG
  QElapsedTimer timer;
  timer.start();
#endif
  auto db = prepare(source.path);
  if (db.databaseName().isEmpty())
    return LinkedCache();
  LinkedCache selection;
  auto *query = new QSqlQuery(db);
  exec(query, SelectAEL, {source.table_name});
  query->first();
  while (query->isValid()) {
    /*! If the expression includes a value from the table... */
    if (input.contains(purify(query->value(1).toString()))) {
      /*! ...then this value is an activator. */
      auto address = query->value(0).toInt();
      auto props = get_additional_properties(&db, source, address);
      auto links = unpack_links(query->value(2).toString());
      auto activator_text = query->value(1).toString();
      auto subquery = new QSqlQuery(db);
      for (auto link : links) {
        auto *expr = new Expression();
        expr->activator_text = activator_text;
        exec(subquery, SelectExpressionByAddress,
             {source.table_name, QString::number(link)});
        subquery->first();
        if (not subquery->isValid()) {
          std::cout << subquery->lastError().text().toStdString() << std::endl;
          delete subquery;
          subquery = nullptr;
          continue;
        };
        expr->reagent_text = subquery->value(0).toString();
        expr->properties = props;
        selection.append(expr);
#ifdef SQLITE_SCANSOURCE_DEBUG
        qDebug() << "SQLite::scan_source: expression \"" + expr.activator_text +
                        "\" : \"" + expr.reagent_text + "\" added";
#endif
      }
      if (subquery)
        delete subquery;
    }
    query->next();
  }
  db.close();
  delete query;
#ifdef SQLITE_SCANSOURCE_DEBUG
  qDebug() << "SQLite::scan_source:" << timer.nsecsElapsed();
#endif
  return selection;
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
  auto db = QSqlDatabase::database();
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
        emit sqlite_error(tr("Database \"%1\" doesn't exist.").arg(path));
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
        emit sqlite_error(tr("Opening error. Error text: ") +
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
        emit sqlite_error(tr("Database \"%1\" doesn't exist.").arg(path));
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
        emit sqlite_error(tr("Opening error. Error text: ") +
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
        emit sqlite_error(tr("Database \"%1\" doesn't exist.").arg(path));
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
        emit sqlite_error(tr("Opening error. Error text: ") +
                          db.lastError().text());
#ifdef SQLITE_AUTO_TESTS
        qDebug() << "\tSQLite::prepare: opening error:"
                 << db.lastError().text();
#endif
      }
      if (result)
        *result = false;
    }
    auto valid = validate(&db, true, quiet);
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
  auto db_suffix = tr("Database \"%1\".").arg(db->databaseName());
  auto query = QSqlQuery(*db);
  exec(&query, IfMainTableExists);
  query.first();
  if (not query.isValid() and not quiet) {
    emit sqlite_error(
        tr("Validation error: table with sources does not exist.") + " " +
        db_suffix);
    return false;
  }
  exec(&query, IfMainTableCorrect);
  if (not query.first() and not quiet) {
    emit sqlite_error(tr("Validation error: table with sources is empty.") +
                      " " + db_suffix);
    return false;
  }
  auto sourceColumnValid = true;
  sourceColumnValid &= query.value(1).toString() == "source";
  sourceColumnValid &= query.value(2).toString() == "TEXT";
  sourceColumnValid &= query.value(3).toInt() == 1;
  if (not sourceColumnValid and not quiet) {
    emit sqlite_error(
        tr("Validation error: the first column of the table with sources does "
           "not fit the description of \"source\" TEXT NOT NULL UNIQUE.") +
        " " + db_suffix);
    return false;
  }
  if (not query.next() and not quiet) {
    emit sqlite_error(tr("Validation error: the table with sources contains "
                         "only one column.") +
                      " " + db_suffix);
    return false;
  }
  auto titleColumnValid = true;
  titleColumnValid &= query.value(1).toString() == "title";
  titleColumnValid &= query.value(2).toString() == "TEXT";
  if (not titleColumnValid and not quiet) {
    emit sqlite_error(
        tr("Validation error: the second column of the table with sources does "
           "not fit the description of \"title\" TEXT.") +
        " " + db_suffix);
    return false;
  }
  if (not query.next() and not quiet) {
    emit sqlite_error(tr("Validation error: the table with sources contains "
                         "only two columns.") +
                      " " + db_suffix);
    return false;
  }
  auto isReadOnlyColumnValid = true;
  isReadOnlyColumnValid &= query.value(1).toString() == "isReadOnly";
  isReadOnlyColumnValid &= query.value(2).toString() == "INTEGER";
  isReadOnlyColumnValid &= query.value(3).toInt() == 1;
  if (not isReadOnlyColumnValid and not quiet) {
    emit sqlite_error(
        tr("Validation error: the third column of the table with sources does "
           "not fit the description of \"isReadOnly\" INTEGER NOT NULL.") +
        " " + db_suffix);
    return false;
  }
  if (not query.next() and not quiet) {
    emit sqlite_error(tr("Validation error: the table with sources contains "
                         "only three columns.") +
                      " " + db_suffix);
    return false;
  }
  auto isPrivateColumnValid = true;
  isPrivateColumnValid &= query.value(1).toString() == "isPrivate";
  isPrivateColumnValid &= query.value(2).toString() == "INTEGER";
  isPrivateColumnValid &= query.value(3).toInt() == 1;
  if (not isPrivateColumnValid and not quiet) {
    emit sqlite_error(
        tr("Validation error: the fourth column of the table with sources does "
           "not fit the description of \"isPrivate\" INTEGER NOT NULL.") +
        " " + db_suffix);
    return false;
  }
  if (not query.next() and not quiet) {
    emit sqlite_error(tr("Validation error: the table with sources contains "
                         "only four columns.") +
                      " " + db_suffix);
    return false;
  }
  auto isCatchingColumnValid = true;
  isCatchingColumnValid &= query.value(1).toString() == "isCatching";
  isCatchingColumnValid &= query.value(2).toString() == "INTEGER";
  isCatchingColumnValid &= query.value(3).toInt() == 1;
  if (not isCatchingColumnValid and not quiet) {
    emit sqlite_error(
        tr("Validation error: the fifth column of the table with sources does "
           "not fit the description of \"isCatching\" INTEGER NOT NULL.") +
        " " + db_suffix);
    return false;
  }
  if (not query.next() and not quiet) {
    emit sqlite_error(tr("Validation error: the table with sources contains "
                         "only five columns.") +
                      " " + db_suffix);
    return false;
  }
  auto isPrioritisedColumnValid = true;
  isPrioritisedColumnValid &= query.value(1).toString() == "isPrioritised";
  isPrioritisedColumnValid &= query.value(2).toString() == "INTEGER";
  isPrioritisedColumnValid &= query.value(3).toInt() == 1;
  if (not isPrioritisedColumnValid and not quiet) {
    emit sqlite_error(
        tr("Validation error: the sixth column of the table with sources does "
           "not fit the description of \"isPrioritised\" INTEGER NOT NULL.") +
        " " + db_suffix);
    return false;
  }
  if (recursive) {
    exec(&query, SelectSources);
    if (not query.first())
      return true;
    auto eachSourceValid = true;
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
  auto db_source_suffix = tr("Database \"%1\", source \"%2\".")
                                 .arg(db->databaseName(), source_table);
  auto query = QSqlQuery(*db);
  exec(&query, IfSourceTableExists, {source_table});
  if (not query.isValid() and not quiet) {
    emit sqlite_error(tr("Validation error: source does not exist.") + " " +
                      db_source_suffix);
    return false;
  }
  exec(&query, IfSourceTableCorrect, {source_table});
  if (not query.first() and not quiet) {
    emit sqlite_error(tr("Validation error: source is empty.") + " " +
                      db_source_suffix);
    return false;
  }
  auto addressColumnValid = true;
  addressColumnValid &= query.value(1).toString() == "address";
  addressColumnValid &= query.value(2).toString() == "INTEGER";
  addressColumnValid &= query.value(3).toInt() == 1;
  addressColumnValid &= query.value(5).toInt() == 1;
  if (not addressColumnValid and not quiet) {
    emit sqlite_error(
        tr("Validation error: the first column of the source does "
           "not fit the description of \"address\" INTEGER NOT "
           "NULL PRIMARY KEY AUTOINCREMENT UNIQUE.") +
        " " + db_source_suffix);
    return false;
  }
  if (not query.next() and not quiet) {
    emit sqlite_error(
        tr("Validation error: the source contains only one column.") + " " +
        db_source_suffix);
    return false;
  }
  auto expressionColumnValid = true;
  expressionColumnValid &= query.value(1).toString() == "expression";
  expressionColumnValid &= query.value(2).toString() == "TEXT";
  if (not expressionColumnValid and not quiet) {
    emit sqlite_error(
        tr("Validation error: the second column of the source does not fit the "
           "description of \"expression\" TEXT.") +
        " " + db_source_suffix);
    return false;
  }
  if (not query.next() and not quiet) {
    emit sqlite_error(
        tr("Validation error: the source contains only two columns.") + " " +
        db_source_suffix);
    return false;
  }
  auto linksColumnValid = true;
  linksColumnValid &= query.value(1).toString() == "links";
  linksColumnValid &= query.value(2).toString() == "TEXT";
  if (not linksColumnValid and not quiet) {
    emit sqlite_error(tr("Validation error: the third column of the source "
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
  case SelectExpressionByAddress:
    query->prepare(QString("SELECT expression FROM \"%1\" WHERE address = :a")
                       .arg(values[0]));
    query->bindValue(":a", values[1].toInt());
    break;
  case SelectAEL:
    query->prepare(QString("SELECT address, expression, links FROM \"%1\"")
                       .arg(values[0]));
    break;
  case SelectELByAddress:
    query->prepare(
        QString("SELECT expression, links FROM \"%1\" WHERE address = :a")
            .arg(values[0]));
    query->bindValue(":a", values[1].toInt());
    break;
  case SelectAPByAddress:
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
    emit sqlite_error(query->lastError().text() + " " + QString::number(int(option)));
    return false;
  }
  return true;
}

/*!
 * @fn SQLite::get_additional_properties
 * @brief Gets additional properties of the expression by @a address.
 * @param[in,out] reference to the QSqlDatabase instance
 * @param[in] source source properties
 * @param[in] address address of expression
 * @returns key-value map of properties
 */
Options SQLite::get_additional_properties(QSqlDatabase *db,
                                          const Source &source, int address) {
  Options props;
  auto *query = new QSqlQuery(*db);
  exec(query, SelectAPByAddress, {source.table_name, QString::number(address)});
  auto record = query->record();
  delete query;
  for (auto row = init_additionals_rows; not record.isNull(row); row++) {
    props.insert(record.fieldName(row), record.value(row).toString());
  }
  return props;
}

/*!
 * @fn SQLite::unpack_links
 * @brief
 * @param links
 * @returns
 */
QSet<int> SQLite::unpack_links(const QString &links) {
  QSet<int> unpacked;
  QStringList splitted = links.split(",");
  for (auto link : splitted)
    unpacked.insert(link.toInt());
  return unpacked;
}
