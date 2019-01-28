#include "asw.h"
#include <QApplication>

int main(int argc, char *argv[]) {
  QApplication asw(argc, argv);
  QCoreApplication::setApplicationName("asw-qt");
  QCoreApplication::setApplicationVersion("0.0.3");
  // Setting default font...
  int ID = QFontDatabase::addApplicationFont(
      ":/arts/fonts/fira-sans-condensed-regular.ttf");
  QFont Font = QFontDatabase::applicationFontFamilies(ID).at(0);
  Font.setPointSize(QApplication::font().pointSize());
  QApplication::setFont(Font);
  // Starting main window...
  ASW window;
  window.show();
  return QApplication::exec();
}
