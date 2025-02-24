#ifndef SQLITE_H
#define SQLITE_H

#include "core-kit/model/nlp/cache.hpp"
#include "core-kit/model/nlp/expression.hpp"
#include "core-kit/model/nlp/stringssearch.hpp"
#include "core-kit/model/phrase.hpp"
#include "core-kit/model/source.hpp"
#include "yelloger.h"
#include <QFile>
#include <QMap>
#include <QMutex>
#include <QObject>
#include <QPair>
#include <QRandomGenerator>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlResult>
#include <QString>
#include <QStringList>
#include <QTime>
#include <QUuid>
#include <QVariant>

/*! @enum Check
 *  @brief Enumeration of possible types of checking the state of database.  */
enum Check { NoCheck, Exists, Openable, Correct, RecursivelyCorrect };

/*! @enum ToDo
 *  @brief Enumeration of possible types of operations with database.  */
enum ToDo {
  NoneToDo,
  CreateMainTableIfNotExists,
  CreateSourceTable,
  LoadOptions,
  WriteOptions,
  SelectSources,
  CountPhrases,
  InsertPhrase,
  SelectPhrases,
  SelectAddressesByExpression,
  SelectAddressesByExpressionAndExec,
  SelectLinksByAddress,
  UpdateLinksByAddress,
  SelectEEPByAddress,
  SelectPhraseByAddress,
  UpdatePhraseByAddress,
  UpdateExecByAddress,
  RemovePhraseByAddress,
  IfMainTableExists,
  IfMainTableCorrect,
  IfSourceTableExists,
  IfSourceTableCorrect,
  RemoveMainTableIfExists,
};

/*! @class SQLite
 *  @brief Contains methods of working with databases.  */
class SQLite : public QObject {
  Q_OBJECT
public:
  // Functions:
  /*! @brief The constructor. */
  SQLite(QObject *parent = nullptr) : QObject(parent) { QSqlDatabase::addDatabase("QSQLITE"); }

  // Functions described in `sqlite.cpp`:
  bool create_source(const Source &source, QString *uuid);
  bool create_base_structure(QSqlQuery *query);
  Sources sources(const QString &path);
  Source load_source(Source source);
  bool write_source(const Source &source);
  Phrases select_all(const Source &source);
  bool insert_expression(const Source &source, const Expression &expression);
  bool insert_phrase(const Source &source, const Phrase &phrase);
  Phrase get_phrase_by_address(const Source &source, int address);
  int create_new_phrase(const Source &source, const QString &text);
  bool update_phrase(const Source &source, const QString &phrase, int address);
  bool update_exec(const Source &source, bool ex, int address);
  bool update_links(const Source &source, QSet<int> links, int address);
  bool remove_phrase(const Source &source, int address);
  CoverageCache scan_source(const Source &source, const QString &input, QString conn_name = "");
  QString generate_uuid();

signals:
  /*! @brief Reports an error in the database. */
  QString sqlite_error(QString error_text);

private:
  // Constants:
  static const int maximum_number_of_attempts = 4; /*!< Number of attempts for table creation. */
  static const int init_additionals_rows = 4;      /*!< Column from which additional expression
                                                        properties begin. */

  // Functions described in `sqlite.cpp`:
  QSqlDatabase prepare(const QString &path, QString conn_name = "", Check option = Openable,
                       bool *result = nullptr, bool quiet = false);
  bool check(QSqlDatabase *db, Check option = NoCheck);
  bool validate(QSqlDatabase *db, bool recursive = false, bool quiet = false);
  bool validate(QSqlDatabase *db, const QString &source_table, bool quiet = false);
  bool exec(QSqlQuery *query, ToDo option, QStringList values = {});
};

#endif
