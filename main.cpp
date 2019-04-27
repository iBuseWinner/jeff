#include "asw.h"

int main(int argc, char *argv[]) {
  QApplication asw(argc, argv);
  QCoreApplication::setApplicationName("asw-qt");
  QCoreApplication::setApplicationVersion("0.1.3");
  // Starting main window...
  ASW w;
  w.show();
  return QApplication::exec();
}
