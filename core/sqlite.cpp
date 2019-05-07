#include "sqlite.h"

sqlite::sqlite(QObject *p) : QObject(p) {}

void sqlite::create(const container &cProp) {
  QPair<QSqlDatabase, bool> db = openDB(cProp.p);
  if (!db.second) return;
  QSqlQuery q;
  q.prepare(QString("CREATE TABLE IF NOT EXISTS \"%1\" (address INTEGER, "
                    "expression TEXT, "
                    "links TEXT)")
                .arg(cProp.c));
  q = execQuery(q);
  createMainTable(q);
  q.prepare("INSERT INTO tables VALUES (:c, :t, :ro, :pv, :ch)");
  q.bindValue(":c", cProp.c);
  q.bindValue(":t", cProp.t);
  q.bindValue(":ro", cProp.ro);
  q.bindValue(":pv", cProp.pv);
  q.bindValue(":ch", cProp.ch);
  q = execQuery(q);
  db.first.close();
}

QList<container> sqlite::containers(const QString &p) {
  QList<container> cProps;
  if (!exists(p)) return cProps;
  QPair<QSqlDatabase, bool> db = openDB(p);
  if (!db.second) return cProps;
  if (isDBEmpty(db.first)) return cProps;
  QStringList cs = db.first.tables();
  if (!cs.contains("tables")) {
    for (QString c : cs) {
      QSqlRecord r = db.first.record(c);
      if ((r.fieldName(0) == "address") && (r.fieldName(1) == "expression") &&
          (r.fieldName(2) == "links")) {
        container cProp;
        cProp.p = p;
        cProp.c = c;
        cProp.t = c;
        cProps.append(cProp);
      }
    }
  } else {
    QSqlQuery q("SELECT * FROM tables");
    q = execQuery(q);
    q.first();
    while (q.isValid()) {
      container cProp;
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
  db.first.close();
  return cProps;
}

container sqlite::optionsLoader(container cProp) {
  if (!exists(cProp.p)) return cProp;
  QPair<QSqlDatabase, bool> db = openDB(cProp.p);
  if (!db.second) return cProp;
  if (isDBEmpty(db.first)) return cProp;
  QSqlQuery q;
  q.prepare(QString("SELECT (title, ro, private, catching) FROM tables WHERE "
                    "container=\"%1\"")
                .arg(cProp.c));
  q = execQuery(q);
  if (!q.first()) return cProp;
  cProp.t = q.value(0).toString();
  cProp.ro = q.value(1).toBool();
  cProp.pv = q.value(2).toBool();
  cProp.ch = q.value(3).toBool();
  db.first.close();
  return cProp;
}

void sqlite::optionsWriter(const container &cProp) {
  if (!exists(cProp.p)) return;
  QPair<QSqlDatabase, bool> db = openDB(cProp.p);
  if (!db.second) return;
  QSqlQuery q(db.first);
  createMainTable(q);
  q.prepare("DELETE FROM tables WHERE container=:c");
  q.bindValue(":c", cProp.c);
  q = execQuery(q);
  q.prepare("INSERT INTO tables VALUES(:c, :t, :ro, :pv, :ch)");
  q.bindValue(":c", cProp.c);
  q.bindValue(":t", cProp.t);
  q.bindValue(":ro", cProp.ro);
  q.bindValue(":pv", cProp.pv);
  q.bindValue(":ch", cProp.ch);
  q = execQuery(q);
  db.first.close();
}

void sqlite::insert(const container &cProp, int a, const QString &e,
                    const QString &ls) {
  if (!exists(cProp.p)) return;
  QPair<QSqlDatabase, bool> db = openDB(cProp.p);
  if (!db.second) return;
  if (isDBEmpty(db.first)) return;
  QSqlQuery q;
  q.prepare(QString("INSERT OR REPLACE INTO \"%1\" VALUES (:a, :ex, :ls)")
                .arg(cProp.c));
  q.bindValue(":a", a);
  q.bindValue(":ex", e);
  q.bindValue(":ls", ls);
  q = execQuery(q);
  db.first.close();
}

QPair<QString, QString> sqlite::scan(const container &cProp, int a) {
  QPair<QString, QString> enls;
  if (!exists(cProp.p)) return enls;
  QPair<QSqlDatabase, bool> db = openDB(cProp.p);
  if (!db.second) return enls;
  if (isDBEmpty(db.first)) return enls;
  QSqlQuery q;
  q.prepare(QString("SELECT expression, links FROM \"%1\" WHERE address = :a")
                .arg(cProp.c));
  q.bindValue(":a", a);
  q = execQuery(q);
  db.first.close();
  q.first();
  if (q.isValid()) {
    enls.first = q.value(0).toString();
    enls.second = q.value(1).toString();
  }
  return enls;
}

bool sqlite::hasAdditionalProperties(const container &cProp) {
  if (!exists(cProp.p)) return false;
  QPair<QSqlDatabase, bool> db = openDB(cProp.p);
  if (!db.second) return false;
  if (isDBEmpty(db.first)) return false;
  QSqlRecord r = db.first.record(cProp.c);
  db.first.close();
  return (r.fieldName(3) != "");
}

QMap<QString, QString> sqlite::scanAP(const container &cProp, int a) {
  QMap<QString, QString> aProps;
  if (!exists(cProp.p)) return aProps;
  QPair<QSqlDatabase, bool> db = openDB(cProp.p);
  if (!db.second) return aProps;
  if (isDBEmpty(db.first)) return aProps;
  QSqlQuery q;
  q.prepare(QString("SELECT * FROM \"%1\" WHERE address = :a").arg(cProp.c));
  q.bindValue(":a", a);
  q = execQuery(q);
  db.first.close();
  QSqlRecord r = q.record();
  int f = 3;
  while (!r.isNull(f)) aProps.insert(r.fieldName(f), r.value(f).toString());
  return aProps;
}

QMap<QString, QString> sqlite::scan(const container &cProp, const QString &e) {
  QMap<QString, QString> enls;
  if (!exists(cProp.p)) return enls;
  QPair<QSqlDatabase, bool> db = openDB(cProp.p);
  if (!db.second) return enls;
  if (isDBEmpty(db.first)) return enls;
  QSqlQuery q;
  q.prepare(QString("SELECT expression, links FROM \"%1\"").arg(cProp.c));
  q = execQuery(q);
  q.first();
  while (q.isValid()) {
    if (e.contains(handlers::purify(q.value(0).toString()),
                   Qt::CaseInsensitive))
      enls.insert(q.value(0).toString(), q.value(1).toString());
    q.next();
  }
  db.first.close();
  return enls;
}

void sqlite::createMainTable(QSqlQuery q) {
  q.prepare(
      "CREATE TABLE IF NOT EXISTS tables (container TEXT, title TEXT, "
      "readOnly BOOL, private BOOL, catching BOOL)");
  execQuery(q);
}

bool sqlite::exists(const QString &p) {
  if (!QFile::exists(p)) {
    emit sqliteError("Database \"" + p + "\" doesn't exist.");
    return false;
  }
  return true;
}

QPair<QSqlDatabase, bool> sqlite::openDB(const QString &p) {
  QSqlDatabase _db = QSqlDatabase::database();
  _db.setDatabaseName(p);
  if (!_db.open()) {
    emit sqliteError(_db.lastError().text());
    return QPair<QSqlDatabase, bool>(_db, false);
  }
  return QPair<QSqlDatabase, bool>(_db, true);
}

bool sqlite::isDBEmpty(QSqlDatabase _db) {
  if (_db.tables().isEmpty()) {
    emit sqliteWarning("Database \"" + _db.databaseName() + "\" is empty.");
    return true;
  }
  return false;
}

QSqlQuery sqlite::execQuery(QSqlQuery q) {
  if (!q.exec()) emit sqliteError(q.lastError().text());
  return q;
}
