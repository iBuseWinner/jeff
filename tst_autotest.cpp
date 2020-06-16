#include "core/source.h"
#include "core/sqlite.h"

#include <QCoreApplication>
#include <QDebug>

class ASWAutoTests {
public:
  ASWAutoTests() {}
  void test_database_create() {
    qDebug() << "AutoTest #1: Create database";
    SQLite *sql = new SQLite();
    Source source;
    source.path = "tst_autotest.asw.db";
    source.tableTitle = "tst_autotest_table";
    QString *table_name = new QString();
    qDebug() << "\tSQL: Database Create:" << sql->create(source, table_name);
    qDebug() << "\tSQL: table_name:" << *table_name;
    source.tableName = *table_name;
    delete table_name;
    delete sql;
  }
};

int main(int argc, char *argv[]) {
  QCoreApplication a(argc, argv);
  ASWAutoTests tests;
  tests.test_database_create();
  return a.exec();
}
