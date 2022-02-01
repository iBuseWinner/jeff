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
  if (expression == expression_editor_cmd) {
    auto *modal_handler = new ModalHandler(this);
    auto *expression_editor = new ExpressionEditor(basis, nullptr, modal_handler);
    Q_UNUSED(expression_editor)
    emit showModalWidget(modal_handler);
    return true;
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
  if (expression.startsWith(fast_append_cmd)) {
    if (expression.length() > fast_append_cmd.length()) {
      QString reagent_text = expression;
      reagent_text.remove(0, fast_append_cmd.length());
      QString activator_text = hp->last_user_message();
      if (not activator_text.isEmpty()) {
        Expression e;
        e.activator_text = activator_text;
        e.reagent_text = reagent_text;
        Source s;
        s.path = basis->read(basis->defaultSourcePath).toString();
        s.table_name = basis->read(basis->defaultSourceContainer).toString();
        basis->sql->insert_expression(s, e);
        basis->cacher->append(e);
      }
    }
    return true;
  }
  return false;
}
