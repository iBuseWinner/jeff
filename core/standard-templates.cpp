#include "standard-templates.h"

/*!
 * @fn StandardTemplates::dialogues
 * @brief Looking for a command in the expression. If found, then shows an ASW
 * dialog box.
 * @param[in] expression processed string
 * @returns boolean variable, found or not
 */
bool StandardTemplates::dialogues(const QString &expression) {
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

/*!
 * @fn StandardTemplates::fastCommands
 * @brief Looking for a fast command in the expression. If found, takes action.
 * @param[in] expression processed string
 * @returns boolean variable, found or not
 */
bool StandardTemplates::fastCommands(const QString &expression) {
  if (expression == "/mm") {
    emit changeMonologueMode();
    return true;
  }
  return false;
}

/*!
 * @fn StandardTemplates::inlineCommands
 * @brief Searches for templates in the response expression.
 * @param[in] expression processed string
 * @returns result expression with additional options
 */
QPair<QString, QStringList>
StandardTemplates::inlineCommands(const QString &expression) {
  if (expression.contains(current_time_tmpl)) {
    QString expression_copy = expression;
    expression_copy.replace(current_time_tmpl,
                            QTime::currentTime().toString("HH:mm"));
    return QPair<QString, QStringList>(expression_copy, QStringList());
  }
  /*! If not found... */
  return QPair<QString, QStringList>(expression, QStringList());
}
