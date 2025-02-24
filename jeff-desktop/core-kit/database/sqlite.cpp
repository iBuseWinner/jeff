#include "sqlite.hpp"

/*! @brief Creates source.
 *  @details The source is created even if the database itself does not exist.  */
bool SQLite::create_source(const Source &source, QString *uuid) {
  bool correct;
  auto db = prepare(source.path, "", Openable, &correct, true);
  QSqlQuery query(db);
  if (not create_base_structure(&query)) {
    emit sqlite_error(tr("Could not create main table."));
    Yellog::Error("Could not create main table in the source.");
    db.close();
    return false;
  }
  /*! Generates a table name. User-entered name may be incorrect for SQLite database. */
  QString uuid_to_verify;
  char cntr = 0;
  auto tables = db.tables();
  do {
    uuid_to_verify = generate_uuid();
    cntr++;
  } while ((tables.contains(uuid_to_verify)) &&
           (cntr < maximum_number_of_attempts)); /*!< Generation is repeated
                                                  *   until a unique UUID is
                                                  *   found. */
  if (tables.contains(uuid_to_verify)) {         /*!< If the generation is unsuccessful,
                                                  *   it throws an error. */
    Yellog::Error("Could not create source, number of attempts exceeded %d.", maximum_number_of_attempts);
    emit sqlite_error(
      tr("Could not create source, number of attempts exceeded ") +
      QString::number(maximum_number_of_attempts) + "."
    );
    db.close();
    return false;
  }
  *uuid = uuid_to_verify; /*!< The UUID is passed by pointer to the object that
                           *   called this method.
                           *   @details This may be necessary for those objects
                           *   that want to continue editing a new table. */
  /*! The following queries write the parameters about the table being created
   *  (the source with the @a source parameters) to the main table and create
   *  the source table with the name generated in the form of UUID @a *uuid. */
  if (not
    (exec(
      &query, WriteOptions, {
        *uuid, source.table_title,
        QString::number(source.weight)
      }) and
     exec(&query, CreateSourceTable, QStringList(*uuid)))
      ) {
    Yellog::Error("Could not write options and create source table in the source.");
    emit sqlite_error(tr("Could not write options and create source table."));
    db.close();
    return false;
  }
  db.close();
  return true;
}

/*! @brief The following queries create a source table if it does not exist, or
 *  recreate it if it is incorrect.  */
bool SQLite::create_base_structure(QSqlQuery *query) {
  bool result = true;
  result &= exec(query, RemoveMainTableIfExists);
  result &= exec(query, CreateMainTableIfNotExists);
  return result;
}

/*! @brief Searches for database sources of activators and reagents. */
Sources SQLite::sources(const QString &path) {
  auto db = prepare(path, "", Openable);
  if (db.databaseName().isEmpty()) {
    db.close();
    return Sources();
  }
  QSqlQuery query(db);
  exec(&query, SelectSources);
  query.first();
  Sources sources;
  while (query.isValid()) {
    Source source;
    source.path = path;
    source.table_name = query.value(0).toString();
    source.table_title = query.value(1).toString();
    source.weight = query.value(2).toInt();
    sources.append(source);
    query.next();
  }
  db.close();
  return sources;
}

/*! @brief Loads source properties from the database. */
Source SQLite::load_source(Source source) {
  auto db = prepare(source.path);
  if (db.databaseName().isEmpty()) return source;
  QSqlQuery query(db);
  exec(&query, LoadOptions, {source.table_name});
  if (not query.first()) return source;
  source.table_title = query.value(0).toString();
  source.weight = query.value(1).toInt();
  db.close();
  return source;
}

/*! @brief Writes source properties into the database. */
bool SQLite::write_source(const Source &source) {
  auto db = prepare(source.path, "", Openable);
  if (db.databaseName().isEmpty()) return false;
  QSqlQuery query(db);
  auto result = exec(&query, WriteOptions,
                     {source.table_name, source.table_title,
                      QString::number(source.weight)});
  db.close();
  return result;
}

/*! @brief Selects all expressions from @a source. */
Phrases SQLite::select_all(const Source &source) {
  auto db = prepare(source.path);
  if (db.databaseName().isEmpty()) {
    db.close();
    return Phrases();
  }
  QSqlQuery query(db);
  exec(&query, SelectPhrases, {source.table_name});
  query.first();
  Phrases phrases;
  while (query.isValid()) {
    Phrase phrase;
    phrase.address = query.value(0).toInt();
    phrase.phrase = query.value(1).toString();
    phrase.links = Phrase::unpack_links(query.value(2).toString());
    phrase.exec = bool(query.value(3).toInt());
    phrase.properties = Phrase::parse_props(query.value(4).toString());
    phrases.append(phrase);
    query.next();
  }
  db.close();
  return phrases;
}

/*! @brief Inserts a new expression into the source.
 *  @details If phrases are already in the database, just adds a link from one phrase to another.  */
bool SQLite::insert_expression(const Source &source, const Expression &expression) {
  auto db = prepare(source.path);
  if (db.databaseName().isEmpty()) {
    db.close();
    return false;
  }
  QSqlQuery query(db);
  exec(&query, CountPhrases, {source.table_name});
  query.first();
  int new_address;
  if (not query.isValid()) new_address = 1;
  else new_address = query.value(0).toInt() + 1;
  exec(&query, SelectAddressesByExpression, {source.table_name, expression.activator_text});
  query.first();
  int activator_expr_addr = -1;
  if (query.isValid()) {
    activator_expr_addr = query.value(0).toInt();
  }
  exec(&query, SelectAddressesByExpressionAndExec,
       {source.table_name, expression.reagent_text, QString::number(expression.exec)});
  query.first();
  int reagent_expr_address = -1;
  if (query.isValid()) {
    reagent_expr_address = query.value(0).toInt();
  }
  bool result = false;
  if (activator_expr_addr > 0) {
    if (reagent_expr_address > 0) {
      exec(&query, SelectLinksByAddress, {source.table_name, QString::number(activator_expr_addr)});
      query.first();
      auto links = Phrase::unpack_links(query.value(0).toString());
      links.insert(reagent_expr_address);
      result = exec(&query, UpdateLinksByAddress, {source.table_name,
                                                   QString::number(activator_expr_addr),
                                                   Phrase::pack_links(links)});
    } else {
      exec(&query, SelectLinksByAddress, {source.table_name, QString::number(activator_expr_addr)});
      query.first();
      auto links = Phrase::unpack_links(query.value(0).toString());
      links.insert(new_address);
      result = exec(&query, UpdateLinksByAddress, {source.table_name,
                                                   QString::number(activator_expr_addr),
                                                   Phrase::pack_links(links)});
      result &= exec(&query, InsertPhrase, {source.table_name, QString::number(new_address),
                                            expression.reagent_text, "",
                                            QString::number(int(expression.exec)),
                                            Phrase::text_props(expression.properties)});
    }
  } else {
    if (reagent_expr_address > 0) {
      result = exec(&query, InsertPhrase, {source.table_name, QString::number(new_address),
                                           expression.activator_text,
                                           QString::number(reagent_expr_address), "0",
                                           Phrase::text_props(expression.properties)});
    } else {
      result = exec(&query, InsertPhrase, {source.table_name, QString::number(new_address),
                                           expression.activator_text,
                                           QString::number(new_address + 1), "0",
                                           Phrase::text_props(expression.properties)});
      result &= exec(&query, InsertPhrase, {source.table_name, QString::number(new_address + 1),
                                            expression.reagent_text, "",
                                            QString::number(int(expression.exec)),
                                            Phrase::text_props(expression.properties)});
    }
  }
  db.close();
  return result;
}

/*! @brief Inserts a new phrase into the source. */
bool SQLite::insert_phrase(const Source &source, const Phrase &phrase) {
  auto db = prepare(source.path);
  if (db.databaseName().isEmpty()) {
    db.close();
    return false;
  }
  QSqlQuery query(db);
  auto result = exec(
    &query, InsertPhrase,
    {
      source.table_name, QString::number(phrase.address), phrase.phrase,
      Phrase::pack_links(phrase.links), QString::number(int(phrase.exec)),
      Phrase::text_props(phrase.properties)
    }
  );
  db.close();
  return result;
}

/*! @brief Finds a phrase by address. */
Phrase SQLite::get_phrase_by_address(const Source &source, int address) {
  auto db = prepare(source.path);
  if (db.databaseName().isEmpty()) {
    db.close();
    return Phrase();
  }
  QSqlQuery query(db);
  exec(&query, SelectPhraseByAddress, {source.table_name, QString::number(address)});
  query.first();
  Phrase phrase;
  if (query.isValid()) {
    phrase.address = address;
    phrase.phrase = query.value(1).toString();
    phrase.links = Phrase::unpack_links(query.value(2).toString());
    phrase.exec = bool(query.value(3).toInt());
    phrase.properties = Phrase::parse_props(query.value(4).toString());
  }
  db.close();
  return phrase;
}

/*! @brief Creates a stub for a new phrase and returns its address.
 *  @details In fact, it reserves an address for creating a new phrase in the GUI.  */
int SQLite::create_new_phrase(const Source &source, const QString &text) {
  auto db = prepare(source.path);
  if (db.databaseName().isEmpty()) {
    db.close();
    return -1;
  }
  QSqlQuery query(db);
  exec(&query, CountPhrases, {source.table_name});
  query.first();
  auto new_id = query.isValid() ? query.value(0).toInt() + 1 : 1;
  auto result = exec(
    &query, InsertPhrase,
    {
      source.table_name, QString::number(new_id), text, "", QString::number(0),
      Phrase::text_props(Phrase::parse_props(QJsonObject()))
    }
  );
  db.close();
  return result ? new_id : -1;
}

/*! @brief Edits the content of a phrase. */
bool SQLite::update_phrase(const Source &source, const QString &phrase, int address) {
  auto db = prepare(source.path);
  if (db.databaseName().isEmpty()) {
    db.close();
    return false;
  }
  QSqlQuery query(db);
  auto result = exec(
    &query, UpdatePhraseByAddress, {source.table_name, phrase, QString::number(address)}
  );
  db.close();
  return result;
}

/*! @brief Edits whether an expression is executable in a script. */
bool SQLite::update_exec(const Source &source, bool ex, int address) {
  auto db = prepare(source.path);
  if (db.databaseName().isEmpty()) {
    db.close();
    return false;
  }
  QSqlQuery query(db);
  auto result = exec(
    &query, UpdateExecByAddress, {source.table_name, QString::number(ex), QString::number(address)}
  );
  db.close();
  return result;
}

/*! @brief Edits links to other phrases. */
bool SQLite::update_links(const Source &source, QSet<int> links, int address) {
  auto db = prepare(source.path);
  if (db.databaseName().isEmpty()) {
    db.close();
    return false;
  }
  QSqlQuery query(db);
  auto result = exec(
    &query, UpdateLinksByAddress, {source.table_name, Phrase::pack_links(links), QString::number(address)}
  );
  db.close();
  return result;
}

/*! @brief Removes a phrase from a table. */
bool SQLite::remove_phrase(const Source &source, int address) {
  auto db = prepare(source.path);
  if (db.databaseName().isEmpty()) {
    db.close();
    return false;
  }
  QSqlQuery query(db);
  auto result = exec(
    &query, RemovePhraseByAddress, {source.table_name, QString::number(address)}
  );
  db.close();
  return result;
}

/*! @brief Finds all activators for the input. */
CoverageCache SQLite::scan_source(const Source &source, const QString &input, QString conn_name) {
  auto db = prepare(source.path, conn_name);
  if (db.databaseName().isEmpty()) {
    db.close();
    return CoverageCache();
  }
  CoverageCache selection;
  QSqlQuery query(db);
  exec(&query, SelectPhrases, {source.table_name});
  query.first();
  while (query.isValid()) {
    if (query.value(3).toBool()) { query.next(); continue; }
    /*! If the expression includes a value from the table... */
    auto indices_and_POC = StringSearch::contains(input, query.value(1).toString());
    if (indices_and_POC.second != 0.0) {
      /*! ...then this value is an activator. */
      auto links = Phrase::unpack_links(query.value(2).toString());
      auto activator_text = query.value(1).toString();
      QSqlQuery subquery(db);
      for (auto link : links) {
        if (link == 0) continue;
        Expression expr;
        expr.activator_text = activator_text;
        exec(&subquery, SelectEEPByAddress, {source.table_name, QString::number(link)});
        subquery.first();
        if (not subquery.isValid()) continue;
        expr.reagent_text = subquery.value(0).toString();
        expr.exec = subquery.value(1).toBool();
        expr.properties = Phrase::parse_props(subquery.value(2).toString());
        ExpressionCoverage ec;
        ec.expression = expr;
        ec.coverage_indices = indices_and_POC.first;
        ec.total_POC = indices_and_POC.second;
        selection.append(ec);
      }
    }
    query.next();
  }
  db.close();
  return selection;
}

/*! @brief Prepares a database for work. */
QSqlDatabase SQLite::prepare(const QString &path, QString conn_name, Check option, bool *result, bool quiet) {
  QSqlDatabase db;
  if (conn_name.isEmpty()) db = QSqlDatabase::database();
  else db = QSqlDatabase::database(conn_name);
  switch (option) {
    case NoCheck: {
      db.setDatabaseName(path);
      db.open();
      break;
    }
    case Exists: {
      if (result) *result = true;
      if (not QFile::exists(path)) {
        Yellog::Error(QString("Database \"%1\" doesn't exist.").arg(path).toStdString().c_str());
        if (not quiet) emit sqlite_error(tr("Database \"%1\" doesn't exist.").arg(path));
        if (result) *result = false;
        break;
      }
      db.setDatabaseName(path);
      db.open();
      break;
    }
    case Openable: {
      if (result) *result = true;
      db.setDatabaseName(path);
      if (not db.open()) {
        Yellog::Error("Opening error. Error text: %s", db.lastError().text().toStdString().c_str());
        if (not quiet) emit sqlite_error(tr("Opening error. Error text: ") + db.lastError().text());
        if (result) *result = false;
      }
      break;
    }
    case Correct: {
      /*! Correct = Exists + Openable + has the correct structure. */
      if (result) *result = true;
      if (not QFile::exists(path)) {
        Yellog::Error(QString("Database \"%1\" doesn't exist.").arg(path).toStdString().c_str());
        if (not quiet) emit sqlite_error(tr("Database \"%1\" doesn't exist.").arg(path));
        if (result) *result = false;
        break;
      }
      db.setDatabaseName(path);
      if (not db.open()) {
        Yellog::Error("Opening error. Error text: %s", db.lastError().text().toStdString().c_str());
        if (not quiet) emit sqlite_error(tr("Opening error. Error text: ") + db.lastError().text());
        if (result) *result = false;
      }
      bool valid = validate(&db, false, quiet);
      if (result) *result = valid;
      break;
    }
    case RecursivelyCorrect: {
      if (result) *result = true;
      if (not QFile::exists(path)) {
        Yellog::Error(QString("Database \"%1\" doesn't exist.").arg(path).toStdString().c_str());
        if (not quiet) emit sqlite_error(tr("Database \"%1\" doesn't exist.").arg(path));
        if (result) *result = false;
        break;
      }
      db.setDatabaseName(path);
      if (not db.open()) {
        Yellog::Error("Opening error. Error text: %s", db.lastError().text().toStdString().c_str());
        if (not quiet) emit sqlite_error(tr("Opening error. Error text: ") + db.lastError().text());
        if (result) *result = false;
      }
      auto valid = validate(&db, true, quiet);
      if (result) *result = valid;
      break;
    }
  }
  return db;
}

/*! @brief Validates the database.
 *  @returns result of validation
 *  @retval true validation successful, no errors found
 *  @retval false error was encountered, the database is invalid  */
bool SQLite::validate(QSqlDatabase *db, bool recursive, bool quiet) {
  auto db_suffix = tr("Database \"%1\".").arg(db->databaseName());
  auto query = QSqlQuery(*db);
  exec(&query, IfMainTableExists);
  query.first();
  if (not query.isValid() and not quiet) {
    Yellog::Error("Validation error: table with sources does not exist. %s", db_suffix.toStdString().c_str());
    emit sqlite_error(tr("Validation error: table with sources does not exist.") + " " + db_suffix);
    return false;
  }
  exec(&query, IfMainTableCorrect);
  if (not query.first() and not quiet) {
    Yellog::Error("Validation error: table with sources is empty. %s", db_suffix.toStdString().c_str());
    emit sqlite_error(tr("Validation error: table with sources is empty.") + " " + db_suffix);
    return false;
  }
  auto sourceColumnValid = true;
  sourceColumnValid &= query.value(1).toString() == "source";
  sourceColumnValid &= query.value(2).toString() == "TEXT";
  sourceColumnValid &= query.value(3).toInt() == 1;
  if (not sourceColumnValid and not quiet) {
    Yellog::Error("Validation error: the first column of the table with sources does not fit the description "
                  "of \"source\" TEXT NOT NULL UNIQUE. %s", db_suffix.toStdString().c_str());
    emit sqlite_error(
      tr("Validation error: the first column of the table with sources does not fit the description of \"source\" TEXT NOT NULL UNIQUE.") + " " + db_suffix
    );
    return false;
  }
  if (not query.next() and not quiet) {
    Yellog::Error("Validation error: the table with sources contains only one column. %s", db_suffix.toStdString().c_str());
    emit sqlite_error(
      tr("Validation error: the table with sources contains only one column.") + " " + db_suffix
    );
    return false;
  }
  auto titleColumnValid = true;
  titleColumnValid &= query.value(1).toString() == "title";
  titleColumnValid &= query.value(2).toString() == "TEXT";
  if (not titleColumnValid and not quiet) {
    Yellog::Error("Validation error: the second column of the table with sources does not fit the description "
                  "of \"title\" TEXT. %s", db_suffix.toStdString().c_str());
    emit sqlite_error(
      tr("Validation error: the second column of the table with sources does not fit the description of \"title\" TEXT.") + " " + db_suffix
    );
    return false;
  }
  if (not query.next() and not quiet) {
    Yellog::Error("Validation error: the table with sources contains only two columns. %s", db_suffix.toStdString().c_str());
    emit sqlite_error(tr("Validation error: the table with sources contains only two columns.") + " " + db_suffix);
    return false;
  }
  auto weightColumnValid = true;
  weightColumnValid &= query.value(1).toString() == "weight";
  weightColumnValid &= query.value(2).toString() == "INTEGER";
  if (not weightColumnValid and not quiet) {
    Yellog::Error("Validation error: the third column of the table with sources does not fit the description "
                  "of \"weight\" INTEGER. %s", db_suffix.toStdString().c_str());
    emit sqlite_error(
      tr("Validation error: the third column of the table with sources does not fit the description of \"weight\" INTEGER.") + " " + db_suffix
    );
    return false;
  }
  if (recursive) {
    exec(&query, SelectSources);
    if (not query.first()) return true;
    auto eachSourceValid = true;
    do eachSourceValid &= validate(db, query.value(0).toString());
    while (query.next());
    return eachSourceValid;
  }
  return true;
}

/*! @brief Validates the database source.
 *  @returns result of validation
 *  @retval true validation successful, no errors found
 *  @retval false error was encountered, the source is invalid  */
bool SQLite::validate(QSqlDatabase *db, const QString &source_table, bool quiet) {
  auto db_source_suffix = tr("Database \"%1\", source \"%2\".").arg(db->databaseName(), source_table);
  auto query = QSqlQuery(*db);
  exec(&query, IfSourceTableExists, {source_table});
  if (not query.isValid() and not quiet) {
    Yellog::Error("Validation error: source does not exist. %s", db_source_suffix.toStdString().c_str());
    emit sqlite_error(tr("Validation error: source does not exist.") + " " + db_source_suffix);
    return false;
  }
  exec(&query, IfSourceTableCorrect, {source_table});
  if (not query.first() and not quiet) {
    Yellog::Error("Validation error: source is empty. %s", db_source_suffix.toStdString().c_str());
    emit sqlite_error(tr("Validation error: source is empty.") + " " + db_source_suffix);
    return false;
  }
  auto addressColumnValid = true;
  addressColumnValid &= query.value(1).toString() == "address";
  addressColumnValid &= query.value(2).toString() == "INTEGER";
  addressColumnValid &= query.value(3).toInt() == 1;
  addressColumnValid &= query.value(5).toInt() == 1;
  if (not addressColumnValid and not quiet) {
    Yellog::Error("Validation error: the fifth column of the source does not fit the description "
                  "of \"address\" INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE. %s", db_source_suffix.toStdString().c_str());
    emit sqlite_error(
      tr("Validation error: the first column of the source does not fit the description of \"address\" INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE.") + " " + db_source_suffix
    );
    return false;
  }
  if (not query.next() and not quiet) {
    Yellog::Error("Validation error: the source contains only one column. %s", db_source_suffix.toStdString().c_str());
    emit sqlite_error(tr("Validation error: the source contains only one column.") + " " + db_source_suffix);
    return false;
  }
  auto expressionColumnValid = true;
  expressionColumnValid &= query.value(1).toString() == "phrase";
  expressionColumnValid &= query.value(2).toString() == "TEXT";
  if (not expressionColumnValid and not quiet) {
    Yellog::Error("Validation error: the fifth column of the source does not fit the description "
                  "of \"phrase\" TEXT. %s", db_source_suffix.toStdString().c_str());
    emit sqlite_error(
      tr("Validation error: the second column of the source does not fit the description of \"phrase\" TEXT.") + " " + db_source_suffix
    );
    return false;
  }
  if (not query.next() and not quiet) {
    Yellog::Error("Validation error: the source contains only two columns. %s", db_source_suffix.toStdString().c_str());
    emit sqlite_error(tr("Validation error: the source contains only two columns.") + " " + db_source_suffix);
    return false;
  }
  auto linksColumnValid = true;
  linksColumnValid &= query.value(1).toString() == "links";
  linksColumnValid &= query.value(2).toString() == "TEXT";
  if (not linksColumnValid and not quiet) {
    Yellog::Error("Validation error: the fifth column of the source does not fit the description "
                  "of \"links\" TEXT. %s", db_source_suffix.toStdString().c_str());
    emit sqlite_error(
      tr("Validation error: the third column of the source does not fit the description of \"links\" TEXT.") + " " + db_source_suffix
    );
    return false;
  }
  if (not query.next() and not quiet) {
    Yellog::Error("Validation error: the source contains only three columns. %s", db_source_suffix.toStdString().c_str());
    emit sqlite_error(tr("Validation error: the source contains only three columns.") + " " + db_source_suffix);
    return false;
  }
  auto execColumnValid = true;
  execColumnValid &= query.value(1).toString() == "exec";
  execColumnValid &= query.value(2).toString() == "INTEGER";
  execColumnValid &= query.value(3).toInt() == 1;
  if (not execColumnValid and not quiet) {
    Yellog::Error("Validation error: the fifth column of the source does not fit the description "
                  "of \"exec\" INTEGER NOT NULL. %s", db_source_suffix.toStdString().c_str());
    emit sqlite_error(
      tr("Validation error: the fourth column of the source does not fit the description of \"exec\" INTEGER NOT NULL.") + " " + db_source_suffix
    );
    return false;
  }
  if (not query.next() and not quiet) {
    Yellog::Error("Validation error: the source contains only four columns. %s", db_source_suffix.toStdString().c_str());
    emit sqlite_error(tr("Validation error: the source contains only four columns.") + " " + db_source_suffix);
    return false;
  }
  auto adpropsColumnValid = true;
  adpropsColumnValid &= query.value(1).toString() == "adprops";
  adpropsColumnValid &= query.value(2).toString() == "TEXT";
  if (not adpropsColumnValid and not quiet) {
    Yellog::Error("Validation error: the fifth column of the source does not fit the description "
                  "of \"adprops\" TEXT. %s", db_source_suffix.toStdString().c_str());
    emit sqlite_error(
      tr("Validation error: the fifth column of the source does not fit the description of \"adprops\" TEXT.") + " " + db_source_suffix
    );
    return false;
  }
  return true;
}

/*! @brief Performs a database query.
 *  @returns SQL query result
 *  @retval true SQL query executed successfully
 *  @retval false SQL query was not executed correctly  */
bool SQLite::exec(QSqlQuery *query, ToDo option, QStringList values) {
  switch (option) {
    case CreateMainTableIfNotExists:
      query->prepare(
        "create table if not exists sources (source text not null unique, title text, weight integer);"
      );
      break;
    case CreateSourceTable:
      query->prepare(
        QString("create table \"%1\" (address integer not null primary key autoincrement unique, phrase text, links text, exec integer not null, adprops text)").arg(values[0])
      );
      break;
    case LoadOptions:
      query->prepare(
        QString("select title, weight from sources where source = '%1'").arg(values[0])
      );
      break;
    case WriteOptions:
      query->prepare("insert or replace into sources values (:s, :t, :w)");
      query->bindValue(":s", values[0]);
      query->bindValue(":t", values[1]);
      query->bindValue(":w", values[2].toInt());
      break;
    case SelectSources:
      query->prepare("select * from sources");
      break;
    case CountPhrases:
      query->prepare("select seq from sqlite_sequence where name = :n");
      query->bindValue(":n", values[0]);
      break;
    case InsertPhrase:
      query->prepare(
        QString("insert or replace into \"%1\" values (:a, :x, :ls, :e, :adp)").arg(values[0])
      );
      query->bindValue(":a", values[1].toInt());
      query->bindValue(":x", values[2]);
      query->bindValue(":ls", values[3]);
      query->bindValue(":e", values[4].toInt());
      query->bindValue(":adp", values[5]);
      break;
    case SelectPhrases:
      query->prepare(QString("select * from \"%1\"").arg(values[0]));
      break;
    case SelectAddressesByExpression:
      query->prepare(QString("select address from \"%1\" where phrase = :x").arg(values[0]));
      query->bindValue(":x", values[1]);
      break;
    case SelectAddressesByExpressionAndExec:
      query->prepare(
        QString("select address from \"%1\" where phrase = :x and exec = :e").arg(values[0])
      );
      query->bindValue(":x", values[1]);
      query->bindValue(":e", values[2].toInt());
      break;
    case SelectLinksByAddress:
      query->prepare(
        QString("select links from \"%1\" where address = :a").arg(values[0])
      );
      query->bindValue(":a", values[1].toInt());
      break;
    case UpdateLinksByAddress:
      query->prepare(QString("update \"%1\" set links = :ls where address = :a").arg(values[0]));
      query->bindValue(":ls", values[1]);
      query->bindValue(":a", values[2].toInt());
      break;
    case SelectEEPByAddress:
      query->prepare(QString(
        "select phrase, exec, adprops from \"%1\" where address = :a").arg(values[0])
      );
      query->bindValue(":a", values[1].toInt());
      break;
    case SelectPhraseByAddress:
      query->prepare(QString(
        "select * from \"%1\" where address = :a").arg(values[0])
      );
      query->bindValue(":a", values[1].toInt());
      break;
    case UpdatePhraseByAddress:
      query->prepare(QString(
        "update \"%1\" set phrase = :ex where address = :a").arg(values[0])
      );
      query->bindValue(":ex", values[1]);
      query->bindValue(":a", values[2].toInt());
      break;
    case UpdateExecByAddress:
      query->prepare(QString(
        "update \"%1\" set exec = :e where address = :a").arg(values[0])
      );
      query->bindValue(":e", values[1].toInt());
      query->bindValue(":a", values[2].toInt());
      break;
    case RemovePhraseByAddress:
      query->prepare(QString(
        "delete from \"%1\" where address = :a").arg(values[0])
      );
      query->bindValue(":a", values[1].toInt());
      break;
    case IfMainTableExists:
      query->prepare("select name from sqlite_master where type='table' and name='sources';");
      break;
    case IfMainTableCorrect:
      query->prepare("pragma table_info('sources');");
      break;
    case IfSourceTableExists:
      query->prepare("select name from sqlite_master where type='table' and name=':n';");
      query->bindValue(":n", values[0]);
      break;
    case IfSourceTableCorrect:
      query->prepare("pragma table_info(':n');");
      query->bindValue(":n", values[0]);
      break;
    case RemoveMainTableIfExists:
      query->prepare("drop table if exists 'sources';");
      break;
    case NoneToDo:;
  }
  if (not query->exec()) {
    Yellog::Error("Error during exec query: \"%s\", option %d.", query->lastError().text().toStdString().c_str(), int(option));
    emit sqlite_error(query->lastError().text() + " " + QString::number(int(option)));
    return false;
  }
  return true;
}

/*! @brief Generates a UUID based on the current time. */
QString SQLite::generate_uuid() {
  QRandomGenerator rand(quint32(QTime::currentTime().msec()));
  auto uuid = QUuid(uint(rand.generate()),       ushort(rand.generate()),     ushort(rand.generate()),
                    uchar(rand.bounded(0, 256)), uchar(rand.bounded(0, 256)), uchar(rand.bounded(0, 256)),
                    uchar(rand.bounded(0, 256)), uchar(rand.bounded(0, 256)), uchar(rand.bounded(0, 256)),
                    uchar(rand.bounded(0, 256)), uchar(rand.bounded(0, 256))).toString();
  Yellog::Trace("Generated UUID: %s", uuid.toStdString().c_str());
  return uuid;
}
