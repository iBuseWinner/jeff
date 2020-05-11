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
  CreateContainerTable,
  WithDraw,
  LoadOptions,
  WriteOptions,
  SelectContainers,
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
  void create(const Source &_source);
  QList<Source> sources(const QString &path);
  Source load(Source _source);
  void write(const Source &_source);
  void insert(const Source &_source, int address, const QString &expression,
              const QString &links);
  QPair<QString, QString> getExpression(const Source &_source, int address);
  QMap<QString, QString> scanSource(const Source &_source,
                                    const QString &expression);
  bool hasAdditionalProperties(const Source &_source);
  QMap<QString, QString> scanAdditionalProperties(const Source &_container,
                                                  int address);

  /*! Class initialization. */
  SQLite(QObject *parent) { setParent(parent); }
  /*! Returns the UUID of the created container. */
  QString getUuid() {
    QString _u = uuid;
    uuid.clear();
    return _u;
  }
  /*! Purifies {str}. */
  QString purify(const QString &str) {
    return removeSymbols(str.trimmed().toLower());
  }
  /*! Removes punctuation. */
  QString removeSymbols(QString str) {
    for (auto sb : sbs)
      str.remove(sb);
    return str;
  }

signals:
  QString sqliteError(QString errorText);
  QString sqliteWarning(QString warningText);

private:
  // Objects:
  int mna = 4;
  QString uuid = "";
  const QString sbs = ".,:;!?-'\"";

  // Functions:
  QSqlDatabase prepare(const QString &path, check o = check::All);
  void exec(QSqlQuery *q, todo o, QStringList vs = QStringList());
};

#endif // SQLITE_H
