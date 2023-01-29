/*!
 * @copyright 2018-2023 Mark CDA <aclo.create@gmail.com>
 * @copyright 2018-2023 Shamshin Victor <androiddeveloper@yandex.ru>
 *
 * MIT License
 *
 * Copyright (c) 2018-2022 Mark CDA, Shamshin Victor
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * @brief The associative system program "Jeff".
 * @note Code notes:
 *    1) public objects, variables, functions and their arguments must have
 * clear names, private objects or those objects that are used within one
 * function may have abbreviated names;
 *    2) it is recommended serving for objects and variables the same purpose
 * throughout the code to give the same names.
 */

#ifdef JEFF_WITH_QT_WIDGETS
#ifndef Q_OS_ANDROID
#include "jeff-ui.hpp"
#include "widgets/styling.hpp"
#endif
#else
#include "jeff.hpp"
#endif

#include <QTranslator>
#include <yelloger.h>

/*! @brief Starts Jeff.  */
int main(int argc, char *argv[]) {
#ifdef YELLOGER_TRACING
  Yellog::SetPriority(Yellog::TracePriority);
#endif
  
  QCoreApplication::setApplicationName("jeff");
  QCoreApplication::setApplicationVersion("2.0.3");
  
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
  emit jeff_daemon->ready_state();
  auto result = jeff.exec();
  if (jeff_daemon) delete jeff_daemon;
  return result;
#endif
}
