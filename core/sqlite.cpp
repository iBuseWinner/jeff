#include "sqlite.h"

void sqlite::create(const containerProperties &ncp) {
  // Creates a database if it does not exist, and a new container if it does not
  // exist either.
  QSqlDatabase db = QSqlDatabase::database();
  db.setDatabaseName(ncp.p);
  db.open();
  QSqlQuery q(db);
  q.exec(QString("CREATE TABLE IF NOT EXISTS \"%1\" (address INTEGER, expression "
                 "TEXT, links TEXT)")
             .arg(ncp.c));
  ct(q);
  q.prepare("INSERT INTO tables VALUES (:c, :t, :ro, :pv, :ch)");
  q.bindValue(":c", ncp.c);
  q.bindValue(":t", ncp.t);
  q.bindValue(":ro", ncp.ro);
  q.bindValue(":pv", ncp.pv);
  q.bindValue(":ch", ncp.ch);
  q.exec();
  db.close();
}

QList<containerProperties> sqlite::containers(const QString &p) {
  // Returns list of existing containers.
  QSqlDatabase db = QSqlDatabase::database();
  db.setDatabaseName(p);
  db.open();
  QStringList cs = db.tables();
  QList<containerProperties> cProps;
  if (!cs.contains("tables")) {
    for (QString c : cs) {
      QSqlRecord r = db.record(c);
      if ((r.fieldName(0) == "address") && (r.fieldName(1) == "expression") &&
          (r.fieldName(2) == "links")) {
        containerProperties cProp;
        cProp.p = p;
        cProp.c = c;
        cProp.t = c;
        cProps.append(cProp);
      }
    }
  } else {
    cs.removeOne("tables");
    QSqlQuery q("SELECT * FROM tables");
    q.exec();
    q.first();
    while (q.isValid()) {
      containerProperties cProp;
      cProp.p = p;
      cProp.c = q.value(0).toString();
      cProp.t = q.value(1).toString();
      cProp.ro = q.value(2).toInt();
      cProp.pv = q.value(3).toInt();
      cProp.ch = q.value(4).toInt();
      cProps.append(cProp);
      q.next();
    }
  }
  db.close();
  return cProps;
}

containerProperties sqlite::optionsLoader(containerProperties cp) {
  QSqlDatabase db = QSqlDatabase::database();
  db.setDatabaseName(cp.p);
  db.open();
  QSqlQuery q(db);
  q.prepare(
      QString("SELECT (title, ro, private, catching) FROM tables WHERE "
              "container=:c"));
  q.bindValue(":c", cp.c);
  q.exec();
  if (!q.first()) return cp;
  cp.t = q.value(0).toString();
  cp.ro = q.value(1).toBool();
  cp.pv = q.value(2).toBool();
  cp.ch = q.value(3).toBool();
  db.close();
  return cp;
}

void sqlite::optionsWriter(const containerProperties &cp) {
  QSqlDatabase db = QSqlDatabase::database();
  db.setDatabaseName(cp.p);
  db.open();
  QSqlQuery q(db);
  ct(q);
  q.prepare("DELETE FROM tables WHERE container=:c");
  q.bindValue(":c", cp.c);
  q.exec();
  q.prepare("INSERT INTO tables VALUES(:c, :t, :ro, :pv, :ch)");
  q.bindValue(":c", cp.c);
  q.bindValue(":t", cp.t);
  q.bindValue(":ro", cp.ro);
  q.bindValue(":pv", cp.pv);
  q.bindValue(":ch", cp.ch);
  q.exec();
  db.close();
}

void sqlite::insert(const QString &p, const QString &c, int a,
                    const QString &ex, const QString &ls) {
  // Inserts a new entry into the container.
  QSqlDatabase db = QSqlDatabase::database();
  db.setDatabaseName(p);
  db.open();
  QSqlQuery q(db);
  q.prepare(QString("INSERT OR REPLACE INTO %1 VALUES (:a, :ex, :ls)").arg(c));
  q.bindValue(":a", a);
  q.bindValue(":ex", ex);
  q.bindValue(":ls", ls);
  q.exec();
  db.close();
}

QPair<QString, QString> sqlite::scan(const QString &p, const QString &c,
                                     int a) {
  // Returns expression and links by address value.
  QSqlDatabase db = QSqlDatabase::database();
  db.setDatabaseName(p);
  db.open();
  QSqlQuery q(db);
  q.prepare(
      QString("SELECT expression, links FROM %1 WHERE address = :a").arg(c));
  q.bindValue(":a", a);
  q.exec();
  QString exs;
  QString ls;
  q.first();
  while (q.isValid()) {
    exs = q.value(0).toString();
    ls = q.value(1).toString();
    q.next();
  }
  db.close();
  return QPair<QString, QString>(exs, ls);
}

bool sqlite::hasAdditionalProperties(const QString &p, const QString &c) {
  // Find out if the table has additional properties.
  QSqlDatabase db = QSqlDatabase::database();
  db.setDatabaseName(p);
  db.open();
  QSqlRecord r = db.record(c);
  db.close();
  return r.fieldName(3) != "";
}

QMap<QString, QString> sqlite::scanAP(const QString &p, const QString &c,
                                      int a) {
  // Returns additional table properties.
  QSqlDatabase db = QSqlDatabase::database();
  db.setDatabaseName(p);
  db.open();
  QSqlQuery q(db);
  q.prepare(QString("SELECT * FROM %1 WHERE address = :a").arg(c));
  q.bindValue(":a", a);
  q.exec();
  db.close();
  QSqlRecord r = q.record();
  int f = 3;
  QMap<QString, QString> aProps;
  while (!r.isNull(f)) aProps.insert(r.fieldName(f), r.value(f).toString());
  return aProps;
}

QList<QPair<QString, QString>> sqlite::scan(const QString &p, const QString &c,
                                            const QString &ex) {
  // Returns activators and links by expression value.
  QSqlDatabase db = QSqlDatabase::database();
  db.setDatabaseName(p);
  db.open();
  handlers hd;
  QSqlQuery q(db);
  q.exec(QString("SELECT expression, links FROM %1").arg(c));
  QList<QPair<QString, QString>> al;
  q.first();
  while (q.isValid()) {
    if (ex.contains(hd.purifyString(q.value(0).toString()),
                    Qt::CaseInsensitive))
      al.append(QPair<QString, QString>(q.value(0).toString(),
                                        q.value(1).toString()));
    q.next();
  }
  db.close();
  return al;
}

void sqlite::ct(QSqlQuery q) {
  q.exec(
      "CREATE TABLE IF NOT EXISTS tables (container TEXT, title TEXT, "
      "readOnly BOOL, private BOOL, catching BOOL)");
}
