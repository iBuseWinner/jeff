#include "standard-templates.h"

/*!
 * Argument: QString {expression} [processed string].
 * Looking for a command in the expression. If found, then shows an ASW
 * dialog box.
 * Returns: found or not.
 */
bool StdTemplates::dialogues(const QString &expression) {
  if (expression == "/about") {
    auto *modal_handler = new ModalHandler(this);
    auto *about = new About(nullptr, modal_handler);
    Q_UNUSED(about)
    emit showModalWidget(modal_handler);
    return true;
  }
  if (expression == "/sm") {
    auto *modal_handler = new ModalHandler(this);
    auto *sources = new SourcesDialog(basis, nullptr, modal_handler);
    Q_UNUSED(sources)
    emit showModalWidget(modal_handler);
    return true;
  }
  if (expression == "/first") {
    auto *modal_handler = new ModalHandler(this);
    auto *first_start = new FirstStart(nullptr, modal_handler);
    Q_UNUSED(first_start)
    emit showModalWidget(modal_handler);
    return true;
  }
  if (expression == "/settings") {
    auto *modal_handler = new ModalHandler(this);
    auto *settings = new Settings(basis, nullptr, modal_handler);
    Q_UNUSED(settings)
    emit showModalWidget(modal_handler);
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
