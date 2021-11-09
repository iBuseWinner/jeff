#include "standard-templates.h"

/*!
 * @fn StandardTemplates::dialogues
 * @brief Looking for a command in the expression. If found, then shows an ASW
 * dialog box.
 * @param[in] expression processed string
 * @returns boolean variable, found or not
 */
bool StandardTemplates::dialogues(const QString &expression) {
  if (expression == about_cmd) {
    auto *modal_handler = new ModalHandler(this);
    auto *about = new About(nullptr, modal_handler);
    Q_UNUSED(about)
    emit showModalWidget(modal_handler);
    return true;
  }
  if (expression == source_manager_cmd) {
    auto *modal_handler = new ModalHandler(this);
    auto *sources = new SourcesDialog(basis, nullptr, modal_handler);
    Q_UNUSED(sources)
    emit showModalWidget(modal_handler);
    return true;
  }
  if (expression == first_start_cmd) {
    auto *modal_handler = new ModalHandler(this);
    auto *first_start = new FirstStart(nullptr, modal_handler);
    Q_UNUSED(first_start)
    emit showModalWidget(modal_handler);
    return true;
  }
  if (expression == settings_cmd) {
    auto *modal_handler = new ModalHandler(this);
    auto *settings = new Settings(basis, nullptr, modal_handler);
    Q_UNUSED(settings)
    emit showModalWidget(modal_handler);
    return true;
  }
  if (expression == fast_append_cmd) {
    
  }
  if (expression == expression_manager_cmd) {
    
  }
  return false;
}

/*!
 * @fn StandardTemplates::fast_commands
 * @brief Looking for a fast command in the expression. If found, takes action.
 * @param[in] expression processed string
 * @returns boolean variable, found or not
 */
bool StandardTemplates::fast_commands(const QString &expression) {
  if (expression == monologue_mode_cmd) {
    emit changeMonologueMode();
    return true;
  }
  return false;
}
