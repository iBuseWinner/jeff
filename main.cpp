#include "asw.h"
#include <QTranslator>

/*!
 * @copyright 2018-2020 Titov Klimenty <aclo.create@gmail.com>, Shamshin Victor
 * <androiddeveloper@yandex.ru>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 *
 * @brief The associative system program.
 * @note Code notes:
 *    1) public objects, variables, functions and their arguments must have
 * clear names, private objects or those objects that are used within one
 * function may have abbreviated names;
 *    2) all short named objects and variables must be explained in the file
 * with class function declarations;
 *    3) it is recommended serving for objects and variables the same purpose
 * throughout the code to give the same names.
 */

/*!
 * @fn main
 * @brief Start ASW.
 * @param argc transmitted automatically by system
 * @param argv transmitted automatically by system
 * @returns 0 if the program was executed successfully
 */
int main(int argc, char *argv[]) {
  QCoreApplication::setApplicationName("asw");
  QCoreApplication::setApplicationVersion("0.3.4");
  QApplication asw(argc, argv);
  auto *asw_l10n = new QTranslator(&asw);
  asw_l10n->load(":/l10n/asw_" + QLocale::system().name());
  asw.installTranslator(asw_l10n);
  class ASW asWindow;
  emit asWindow.ready_state();
  asWindow.show();
  return QApplication::exec();
}
