#ifndef SQLITE_H
#define SQLITE_H

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
#include "core/container.h"
#include "core/handlers.h"

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
 * Class: sqlite
 * Contains methods of working with databases.
 */
class sqlite : public QObject {
  Q_OBJECT
 public:
  // Functions:
  void create(const container &_container);
  QList<container> containers(const QString &path);
  container load(container _container);
  void write(const container &_container);
  void insert(const container &_container, int address,
              const QString &expression, const QString &links);
  QPair<QString, QString> getExpression(const container &_container,
                                        int address);
  QMap<QString, QString> scanContainer(const container &_container,
                                       const QString &expression);
  bool hasAdditionalProperties(const container &_container);
  QMap<QString, QString> scanAdditionalProperties(const container &_container,
                                                  int address);

  /*! Class initialization. */
  sqlite(QObject *parent = nullptr) { setParent(parent); }

  /*! Returns the UUID of the created container. */
  QString getUuid() {
    QString _u = uuid;
    uuid.clear();
    return _u;
  }

 signals:
  QString sqliteError(QString errorText);
  QString sqliteWarning(QString warningText);

 private:
  // Objects:
  int mna = 4;
  QString uuid = "";

  // Functions:
  QSqlDatabase prepare(const QString &path, check o = check::All);
  void exec(QSqlQuery *q, todo o, QStringList vs = QStringList());
};

#endif  // SQLITE_H
