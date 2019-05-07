#include "asw.h"

int main(int argc, char *argv[]) {
  QApplication asw(argc, argv);
  QCoreApplication::setApplicationName("asw");
  QCoreApplication::setApplicationVersion("0.2.0");
  class ASW w;
  w.show();
  return QApplication::exec();
}
