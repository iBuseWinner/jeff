#include "asw.h"
#include <QApplication>

int main(int argc, char *argv[]) {
  QApplication asw(argc, argv);
  QCoreApplication::setApplicationName("asw-qt");
  QCoreApplication::setApplicationVersion("0.0.1");
  ASW window;
  window.show();
  return asw.exec();
}
