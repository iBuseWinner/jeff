#include "standard-templates.hpp"

// #ifdef JEFF_WITH_QT_WIDGETS
/*! @brief Looking for a command in the expression. If found, then shows a Jeff dialog box. */
bool StandardTemplates::dialogues(const QString &expression) {
  if (expression == Basis::about_cmd) {
    auto *modal_handler = new ModalHandler(this);
    auto *about = new About(nullptr, modal_handler);
    Q_UNUSED(about)
    emit showModalWidget(modal_handler);
    return true;
  }
  if (expression == Basis::help_cmd) {
    auto *modal_handler = new ModalHandler(this);
    auto *help = new Help(nullptr, modal_handler);
    Q_UNUSED(help)
    emit showModalWidget(modal_handler);
    return true;
  }
  if (expression == Basis::source_manager_cmd) {
    auto *modal_handler = new ModalHandler(this);
    auto *sources_editor = new SourcesEditor(basis, nullptr, modal_handler);
    Q_UNUSED(sources_editor)
    emit showModalWidget(modal_handler);
    return true;
  }
  if (expression == Basis::first_start_cmd) {
    auto *modal_handler = new ModalHandler(this);
    auto *first_start = new FirstStart(nullptr, modal_handler);
    Q_UNUSED(first_start)
    emit showModalWidget(modal_handler);
    return true;
  }
  if (expression == Basis::settings_cmd) {
    auto *modal_handler = new ModalHandler(this);
    auto *settings = new Settings(basis, nullptr, modal_handler);
    Q_UNUSED(settings)
    emit showModalWidget(modal_handler);
    return true;
  }
  if (expression == Basis::phrase_editor_cmd) {
    auto *modal_handler = new ModalHandler(this);
    auto *phrase_editor = new PhraseEditor(basis, nullptr, modal_handler);
    Q_UNUSED(phrase_editor)
    emit showModalWidget(modal_handler);
    return true;
  }
  if (expression == Basis::extensions_viewer_cmd) {
    auto *modal_handler = new ModalHandler(this);
    auto *extensions_viewer = new ExtensionsViewer(em, nullptr, modal_handler);
    connect(extensions_viewer, &ExtensionsViewer::show_info, this, [this](QString text) { emit show_info(text); });
    Q_UNUSED(extensions_viewer)
    emit showModalWidget(modal_handler);
    return true;
  }
  if (expression == Basis::add_scanner_cmd) {
    auto *modal_handler = new ModalHandler(this);
    auto *add_script = new AddScriptDialog(nullptr, modal_handler, basis);
    add_script->script_editor->set_stype(2);
    Q_UNUSED(add_script)
    emit showModalWidget(modal_handler);
    return true;
  }
  if (expression == Basis::add_composer_cmd) {
    auto *modal_handler = new ModalHandler(this);
    auto *add_script = new AddScriptDialog(nullptr, modal_handler, basis);
    add_script->script_editor->set_stype(3);
    Q_UNUSED(add_script)
    emit showModalWidget(modal_handler);
    return true;
  }
  return false;
}
// #endif

/*! @brief Looking for a fast command in the expression. If found, takes an action. */
bool StandardTemplates::fast_commands(const QString &expression) {
  if (expression == Basis::monologue_mode_cmd) {
    emit changeMonologueMode();
    return true;
  }
  if (expression.startsWith(Basis::fast_append_cmd)) {
    if (expression.length() > QString(Basis::fast_append_cmd).length()) {
      QString reagent_text = expression;
      reagent_text.remove(0, QString(Basis::fast_append_cmd).length());
      QString activator_text = hp->last_user_message(); /*!< This works 'cause in this moment @a expression is not in history. */
      if (not activator_text.isEmpty()) {
        Expression e;
        e.activator_text = activator_text;
        e.reagent_text = reagent_text;
        Source s;
        s.path = (*basis)[Basis::defaultSourcePath].toString();
        s.table_name = (*basis)[Basis::defaultSourceContainer].toString();
        basis->sql->insert_expression(s, e);
        basis->cacher->append(e);
        emit show_info(tr("Expression added successfully!"));
      }
    }
    return true;
  }
  if (expression.startsWith(Basis::fast_append_script_cmd)) {
    if (expression.length() > QString(Basis::fast_append_script_cmd).length()) {
      QString reagent_text = expression;
      reagent_text.remove(0, QString(Basis::fast_append_script_cmd).length());
      QString activator_text = hp->last_user_message();
      if (not activator_text.isEmpty()) {
        Expression e;
        e.activator_text = activator_text;
        e.reagent_text = reagent_text;
        e.exec = true;
        Source s;
        s.path = (*basis)[Basis::defaultSourcePath].toString();
        s.table_name = (*basis)[Basis::defaultSourceContainer].toString();
        basis->sql->insert_expression(s, e);
        basis->cacher->append(e);
      }
    }
    return true;
  }
  if (expression == (*basis)[Basis::scenarioExitMsg]) {
    emit shutdown_scenario();
    return true;
  }
  if (expression == Basis::stop_scanner_cmd)  return true;
  if (expression == Basis::stop_composer_cmd) return true;
  if (expression.startsWith(Basis::appeal_cmd)) {
    auto extension_name = expression.split(' ')[0].mid(1);
    if (extension_name.isEmpty()) return false;
    auto text = expression.mid(2 + extension_name.length()); /*!< @note an empty appeal is the appeal. */
    emit send_to_extension(extension_name, text);
    return true;
  }
  return false;
}
