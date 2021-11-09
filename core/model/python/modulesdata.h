#ifndef MODULES_DATA
#define MODULES_DATA

#include <QList>
#include <QString>

/*!
 * @struct ModulesData
 * @brief Contains data about scripts connected by the user to Jeff.
 */
struct ModulesData {
  QStringList startup_paths;
  QList<QPair<QString, QString>> fast_commands_and_paths;
  QStringList custom_scan_paths;
}

#endif
