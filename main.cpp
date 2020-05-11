#include <QTranslator>
#include "asw.h"

/*
 * All short named objects and their explanations:
 * {asw} <- Associative System Window
 * {asw_l10n} <- ASW localization
 * {w} <- window
 */

/*
 * The associative system program.
 *
 * Code notes:
 *    1) parents {*parent} are transferred to objects so that objects are
 * destroyed when the parent is destroyed;
 *    2) public objects, variables, functions and their arguments must have
 * clear names, private objects or those objects that are used within one
 * function may have abbreviated names;
 *    3) all short named objects and variables must be explained in the file
 * with class function declarations;
 *    4) it is recommended throughout the code for objects and variables serving
 * the same purpose to give the same names.
 */

/*!
 * Arguments: int {argc} [transmitted automatically by system],
 *            char {*argv[]} [transmitted automatically by system].
 * Starts the program.
 * Returns: 0 if the program was successful.
 */
int main(int argc, char *argv[]) {
  QCoreApplication::setApplicationName("asw");
  QCoreApplication::setApplicationVersion("0.2.3");
  QApplication asw(argc, argv);
  auto *asw_l10n = new QTranslator(&asw);
  asw_l10n->load(":/l10n/asw_" + QLocale::system().name());
  asw.installTranslator(asw_l10n);
  class ASW w;
  w.show();
  return QApplication::exec();
}
