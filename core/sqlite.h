#ifndef SQLITE_H
#define SQLITE_H

#include "core/source.h"
#include <QDebug>
#include <QFile>
#include <QJsonObject>
#include <QMap>
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

/*! Enum: check [what to check]. */
enum check { All, OnlyOpen, AnyContent };

/*! Enum: todo [what to do with db]. */
enum todo {
  CreateMainTable,
  CreateSourceTable,
  WithDraw,
  LoadOptions,
  WriteOptions,
  SelectSources,
  InsertExpression,
  SelectExpressionsAndLinks,
  SelectExpressionAndLinksByAddress,
  SelectAdditionalProperties
};

/*!
 * Class: SQLite
 * Contains methods of working with databases.
 */
class SQLite : public QObject {
  Q_OBJECT
public:
  // Functions:
#ifdef SQLITE_AUTO_TESTS
  virtual
#endif
      bool
      create(const Source &_source, QString *uuid);
  QList<Source> sources(const QString &path);
  Source load(Source _source);
  void write(const Source &_source);
  void insert(const Source &_source, int address, const QString &expression,
              const QString &links);
  QPair<QString, QString> getExpression(const Source &_source, int address);
  QMap<QString, QString> scanSource(const Source &_source,
                                    const QString &expression);
  bool hasAdditionalProperties(const Source &_source);
  QMap<QString, QString> scanAdditionalProperties(const Source &_source,
                                                  int address);

  /*! Class initialization. */
  SQLite(QObject *parent = nullptr) : QObject(parent) {
    QSqlDatabase::addDatabase("QSQLITE");
  }
  /*! Purifies {str}. */
  QString purify(const QString &str) {
    return removeSymbols(str.trimmed().toLower());
  }
  /*! Removes punctuation. */
  QString removeSymbols(QString str) {
    for (auto symbol : punctuation_symbols)
      str.remove(symbol);
    return str;
  }

signals:
  QString sqliteError(QString errorText);
  QString sqliteWarning(QString warningText);

private:
  // Objects:
  static const int maximum_number_of_attempts = 4;
  inline static const QString punctuation_symbols = ".,:;!?-'\"";

  // Functions:
  QSqlDatabase prepare(const QString &path, check o = check::All);
  bool exec(QSqlQuery *query, todo option, QStringList values = QStringList());
};

#endif // SQLITE_H
