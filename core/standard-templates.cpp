#include "standard-templates.h"

/*!
 * Argument: QString {expression} [processed string].
 * Looking for a command in the expression. If found, then shows an ASW
 * dialog box.
 * Returns: found or not.
 */
bool StdTemplates::dialogues(const QString &expression) {
  if (expression == "/about") {
    emit showASWDialog(new About());
    return true;
  }
  if (expression == "/sm") {
    emit showASWDialog(new SourcesDialog(basis));
    return true;
  }
  if (expression == "/first") {
    emit showASWDialog(new FirstStart());
    return true;
  }
  if (expression == "/settings") {
    emit showASWDialog(new Settings(basis));
    return true;
  }
  return false;
}

bool StdTemplates::fastCommands(const QString &expression) {
  if (expression == "/mm") {
    emit changeMonologueMode();
    return true;
  }
  return false;
}
