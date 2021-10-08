#include "core.h"

/*!
 * @fn Core::Core
 * @brief The constructor.
 * @details Creates connections between modules and output collectors.
 * @param[in,out] parent QObject parent
 */
Core::Core(QObject *parent) : QObject(parent) {
  //  connect(basis, &Basis::json_error, this, &Core::got_error);
  connect(basis, &Basis::settings_warning, this, &Core::got_warning);
  basis->check_settings_file();
  connect(history_processor, &HProcessor::send_message_history, this,
          &Core::show_history);
  connect(basis->sql, &SQLite::sqlite_error, this, &Core::got_error);
  connect(basis->sql, &SQLite::sqlite_warning, this, &Core::got_warning);
  connect(_standard_templates, &StandardTemplates::showModalWidget, this,
          &Core::got_modal);
  connect(_nlp, &NLPmodule::response, this, &Core::got_message_from_nlp);
  connect(_nlp, &NLPmodule::response_wo, this, &Core::got_message_wo_from_nlp);
  connect(_standard_templates, &StandardTemplates::changeMonologueMode, this,
          [this] { set_monologue_enabled(not _monologue_enabled); });
  set_monologue_enabled((*basis)[basis->isMonologueModeEnabledSt].toBool());
}

/*!
 * @fn Core::~Core
 * @brief The destructor.
 * @details Method which tells delete NLPmodule instance first because of
 * SYSSEGV if Basis instance will be deleted first.
 * @sa NLPmodule, Json, Basis
 */
Core::~Core() { delete _nlp; }

/*!
 * @fn Core::got_message_from_user
 * @brief Handles input @a user_expression, displays a message on the screen and
 * launches modules.
 * @param[in] user_expression contains user input
 */
void Core::got_message_from_user(const QString &user_expression) {
  /*! Does not respond to blank input. */
  if (user_expression.isEmpty())
    return;
  /*! Displays the entered message on the screen. */
  MessageData message = get_message(user_expression, Author::User,
                                ContentType::Markdown, Theme::Std);
  history_processor->append(message);
  emit show(new Message(message));
  /*! If a user has entered the command, there is no need to run other modules.
   */
  if (_standard_templates->dialogues(user_expression))
    return;
  if (_standard_templates->fastCommands(user_expression))
    return;
  _nlp->search_for_suggests(user_expression);
}

/*!
 * @fn Core::got_message_from_nlp
 * @brief Processes the output of the NLP module @a result_expression and
 * displays a message on the screen.
 * @param[in] result_expression contains the response of the NLP module to user
 * input
 */
void Core::got_message_from_nlp(const QString &result_expression) {
  if (result_expression.isEmpty())
    return;
  MessageData message = get_message(result_expression, Author::Jeff,
                                ContentType::Markdown, Theme::Std);
  history_processor->append(message);
  /*! Delay is triggered if enabled. */
  QTimer::singleShot(
      (*basis)[basis->isDelayEnabledSt].toBool()
          ? QRandomGenerator().bounded((*basis)[basis->minDelaySt].toInt(),
                                       (*basis)[basis->maxDelaySt].toInt())
          : 0,
      this, [this, message, result_expression] {
        emit show(new Message(message));
        if (_monologue_enabled)
          _nlp->search_for_suggests(result_expression);
      });
}

/*!
 * @fn Core::got_message_wo_from_nlp
 * @brief Processes the output of the NLP module @a result_expression_wo and
 * displays a message on the screen or do something else according to options.
 * @param[in] result_expression_wo contains the response of the NLP module to
 * user input
 */
void Core::got_message_wo_from_nlp(ResponseWO result_expression_wo) {
  if (result_expression_wo.first.isEmpty() and
      result_expression_wo.second.isEmpty())
    return;
}

/*!
 * @fn Core::got_warning
 * @brief Displays @a warning_text.
 * @param[in] warning_text contains the warning text from some module
 */
void Core::got_warning(const QString &warning_text) {
  /*! The warning color is yellow. */
  MessageData message = get_message(warning_text, Author::Jeff,
                                ContentType::Warning, Theme::Yellow);
  history_processor->append(message);
  emit show(new Message(message));
}

/*!
 * @fn Core::got_error
 * @brief Displays @a errorText.
 * @param[in] error_text contains the error text from some module
 */
void Core::got_error(const QString &error_text) {
  /*! The error color is red. */
  MessageData message =
      get_message(error_text, Author::Jeff, ContentType::Error, Theme::Red);
  history_processor->append(message);
  emit show(new Message(message));
}

/*!
 * @fn Core::got_modal
 * @brief Creates Message @a message_widget, inserts a widget into it and
 * displays a message.
 * @param[in,out] m_handler handler with widget which should be displayed
 * @sa ModalHandler
 */
void Core::got_modal(ModalHandler *m_handler) {
  MessageData message = get_message(m_handler->getPrisoner()->objectName(),
                                Author::Jeff, ContentType::Widget, Theme::Std);
  history_processor->append(message);
  auto *message_widget = new Message(message);
  m_handler->getPrisoner()->setParent(message_widget);
  m_handler->getPrisoner()->setFixedWidth(Message::maximalMessageWidth);
  message_widget->setWidget(m_handler);
  emit show(message_widget);
}

/*!
 * @fn Core::show_history
 * @brief Displays all messages from @a message_histor} on the screen.
 * @param[in] message_history
 */
void Core::show_history(Messages message_history) {
  for (const auto &message : message_history)
    emit show(new Message(message));
}

/*!
 * @fn Core::get_message
 * @brief Creates @a message.
 * @param[in] content content of message
 * @param[in] author who is author - user or asw?
 * @param[in] content_type type of given content
 * @param[in] theme appearance of the message
 * @returns MessageData with given parameters.
 */
MessageData Core::get_message(const QString &content, Author author,
                          ContentType content_type, Theme theme) {
  MessageData message;
  message.content = content;
  message.datetime = QDateTime::currentDateTime();
  message.author = author;
  message.content_type = content_type;
  message.theme = theme;
  return message;
}

/*!
 * @fn Core::set_monologue_enabled
 * @brief Sets the status of the monologue mode.
 * @param[in] enabled boolean value of monologue mode state
 */
void Core::set_monologue_enabled(const bool enabled) {
  _monologue_enabled = enabled;
  emit changeMenuBarMonologueCheckbox(enabled);
}
