#ifdef JEFF_WITH_QT_WIDGETS
#include "jeff-ui.h"
#include "widgets/styling.h"
#else
#include "jeff.h"
#endif

#include <QTranslator>

/*!
 * @copyright 2018-2022 Mark CDA <markcda@protonmail.com>
 * @copyright 2018-2022 Shamshin Victor <androiddeveloper@yandex.ru>
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
 * @brief The associative system program "Jeff".
 * @note Code notes:
 *    1) public objects, variables, functions and their arguments must have
 * clear names, private objects or those objects that are used within one
 * function may have abbreviated names;
 *    2) it is recommended serving for objects and variables the same purpose
 * throughout the code to give the same names.
 */

/*! @fn main
 *  @brief Starts Jeff.  */
int main(int argc, char *argv[]) {
  QCoreApplication::setApplicationName("jeff");
  QCoreApplication::setApplicationVersion("0.5.2");
#ifdef JEFF_WITH_QT_WIDGETS
  QApplication jeff(argc, argv);
  styling.calculate_colors();
#else
  QCoreApplication jeff(argc, argv);
#endif
  auto *jeff_locals = new QTranslator(&jeff);
  jeff_locals->load(":/l10n/jeff_" + QLocale::system().name());
  jeff.installTranslator(jeff_locals);
#ifdef JEFF_WITH_QT_WIDGETS
  class Jeff jeff_window;
  emit jeff_window.ready_state();
  jeff_window.show();
  return QApplication::exec();
#else
  auto *jeff_daemon = new class Jeff(argc, argv);
  auto result = QCoreApplication::exec();
  if (jeff_daemon) delete jeff_daemon;
  return result;
#endif
}
