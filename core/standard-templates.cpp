#include "standard-templates.h"

/*!
 * Argument: QString {expression} [processed string].
 * Looking for a command in the expression. If found, then shows an ASW
 * dialog box.
 * Returns: found or not.
 */
bool standardTemplates::dialogues(const QString &expression) {
  if (expression == "/about") {
    emit showASWDialog(new About());
    return true;
  }
  if (expression == "/cm") {
    emit showASWDialog(new Containers(Meths));
    return true;
  }
  if (expression == "/first") {
    emit showASWDialog(new FirstStart());
    return true;
  }
  return false;
}
