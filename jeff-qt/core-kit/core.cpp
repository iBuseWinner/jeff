#include "core.h"

/*! @brief The constructor. */
Core::Core(QObject *parent) : QObject(parent) {
  connect(basis, &Basis::warn, this, &Core::got_warning);
  connect(basis, &Basis::send, this, &Core::got_message_from_script);
  connect(basis, &Basis::search_again, this, &Core::got_message_to_search_again);
  connect(basis, &Basis::send_as_user, this, &Core::got_message_from_script_as_user);
  connect(basis, &Basis::send_status, this, &Core::got_status_from_script);
  connect(basis, &Basis::send_msg_to_scenario, this, &Core::notify_scenario_first_time);
  basis->check_settings_file();
  basis->check_default_source();
  connect(server, &Server::server_error, this, &Core::got_error);
  connect(sem, &SEModule::script_exception, this, &Core::got_warning);
  connect(basis->sql, &SQLite::sqlite_error, this, &Core::got_error);
#ifdef JEFF_WITH_QT_WIDGETS
  connect(std_templates, &StandardTemplates::showModalWidget, this, &Core::got_modal);
#endif
  connect(jck, &JCKController::response, this, &Core::got_message_from_jck);
  connect(jck, &JCKController::setup_scenario, this, &Core::got_scenario_start);
  connect(std_templates, &StandardTemplates::shutdown_scenario, this, &Core::got_scenario_shutting);
  connect(std_templates, &StandardTemplates::changeMonologueMode, this,
          [this] { set_monologue_enabled(not monologue_enabled); });
  set_monologue_enabled((*basis)[basis->isMonologueEnabledSt].toBool());
  server->start();
}

/*! @brief The destructor. */
Core::~Core() {
  delete jck;
  delete sem;
  delete notifier;
  if ((*basis)[basis->isHistoryKeepingEnabledSt].toBool()) hp->save();
  delete hp;
  delete std_templates;
  server->stop();
  delete server;
  delete basis;
  if (custom_scanner) delete custom_scanner;
  if (custom_composer) delete custom_composer;
}

/*! @brief Handles input @a user_expression, displays a message on the screen and
 *  launches modules.  */
void Core::got_message_from_user(const QString &user_expression) {
  /*! Does not respond to blank input. */
  if (user_expression.isEmpty()) return;
  /*! Displays the entered message on the screen. */
  MessageData message = get_message(user_expression, Author::User, ContentType::Markdown, Theme::Std);
  hp->append(message);
  emit show(message);
  /*! If user has entered any command, there is no need to run other modules. */
#ifdef JEFF_WITH_QT_WIDGETS
  if (std_templates->dialogues(user_expression)) return;
#endif
  if (std_templates->fast_commands(user_expression)) return;
  notifier->notify(message); /*!< Notifies only when message isn't any dialog or command. */
  if (current_scenario) return; /*!< @sa ScenarioScript */
  jck->search_for_suggests(user_expression);
}

/*! @brief Processes the output of the JCK module @a result_expression and
 *  displays a message on the screen.  */
void Core::got_message_from_jck(const QString &result_expression) {
  if (result_expression.isEmpty()) return;
  MessageData mdata = get_message(result_expression, Author::Jeff, ContentType::Markdown, Theme::Std);
  /*! Delay is triggered if enabled. */
  QTimer::singleShot(
      (*basis)[basis->isDelayEnabledSt].toBool()
          ? QRandomGenerator().bounded((*basis)[basis->minDelaySt].toInt(),
                                       (*basis)[basis->maxDelaySt].toInt())
          : 0,
      this, [this, mdata, result_expression] {
        hp->append(mdata);
        notifier->notify(mdata);
        emit show(mdata);
        /*! Search again if monologue mode enabled. */
        if (monologue_enabled) jck->search_for_suggests(result_expression);
      });
}

/*! @brief Runs a scenario process and sets notifications. */
void Core::got_scenario_start(ScenarioScript *scenario) {
  current_scenario = scenario;
  notifier->set_scenario_listener(current_scenario);
  sem->add_daemon(current_scenario);
}

/*! @brief Disables scenario. */
void Core::got_scenario_shutting() {
  notifier->unset_scenario_listener();
  sem->remove_daemon(current_scenario);
  delete current_scenario;
  current_scenario = nullptr;
}

/*! @brief Responds to the scenario's ready message by sending it the last user message. */
void Core::notify_scenario_first_time() {
  if (not current_scenario) return;
  auto recent = hp->recent(1);
  if (recent.isEmpty()) return;
  notifier->notify(recent[0]);
}

/*! @brief Shows output from script or outter app on the screen. */
void Core::got_message_from_script(const QString &message) {
  if (message.isEmpty()) return;
  MessageData mdata = get_message(message, Author::Jeff, ContentType::Markdown, Theme::Std);
  /*! Delay is triggered if enabled. */
  QTimer::singleShot(
      (*basis)[basis->isDelayEnabledSt].toBool()
          ? QRandomGenerator().bounded((*basis)[basis->minDelaySt].toInt(),
                                       (*basis)[basis->maxDelaySt].toInt())
          : 0,
      this, [this, mdata, message] {
        hp->append(mdata);
        notifier->notify(mdata);
        emit show(mdata);
        /*! Search again if monologue mode enabled. */
        if (monologue_enabled) jck->search_for_suggests(message);
      });
}

/*! @brief Searches again. */
void Core::got_message_to_search_again(const QString &rephrased_message) {
  if (rephrased_message.isEmpty()) return;
#ifdef JEFF_WITH_QT_WIDGETS
  if (std_templates->dialogues(rephrased_message)) return;
#endif
  if (std_templates->fast_commands(rephrased_message)) return;
  jck->search_for_suggests(rephrased_message);
}

/*! @brief Shows the message and searches again. */
void Core::got_message_from_script_as_user(const QString &message) {
  /*! Does not respond to blank input. */
  if (message.isEmpty()) return;
  /*! Displays the entered message on the screen. */
  MessageData mdata = get_message(message, Author::User, ContentType::Markdown, Theme::Blue);
  hp->append(mdata);
  notifier->notify(mdata);
  emit show(mdata);
  /*! If a user has entered the command, there is no need to run other modules. */
#ifdef JEFF_WITH_QT_WIDGETS
  if (std_templates->dialogues(message)) return;
#endif
  if (std_templates->fast_commands(message)) return;
  jck->search_for_suggests(message);
}

/*! @brief Shows updateable message. @details Checks id's length. */
void Core::got_status_from_script(QPair<QString, QString> id_and_message) {
  if (id_and_message.first.isEmpty() or id_and_message.second.isEmpty()) return;
  if (id_and_message.first.length() < 24) return;
  MessageData mdata = get_message(id_and_message.second, Author::Jeff,
                                  ContentType::Markdown, Theme::Std);
  /*! Delay is triggered if enabled. */
  QTimer::singleShot(
      (*basis)[basis->isDelayEnabledSt].toBool()
          ? QRandomGenerator().bounded((*basis)[basis->minDelaySt].toInt(),
                                       (*basis)[basis->maxDelaySt].toInt())
          : 0,
      this, [this, mdata, id_and_message] {
        notifier->notify(mdata);
        QPair<QString, MessageData> id_and_message_data(id_and_message.first, mdata);
        emit show_status(id_and_message_data);
      });
}

/*! @brief Displays @a warning_text. */
void Core::got_warning(const QString &warning_text) {
  /*! The warning color is yellow. */
  MessageData message = get_message(warning_text, Author::Jeff, ContentType::Warning, Theme::Yellow);
  hp->append(message);
  notifier->notify(message);
  emit show(message);
}

/*! @brief Displays @a errorText. */
void Core::got_error(const QString &error_text) {
  /*! The error color is red. */
  MessageData message = get_message(error_text, Author::Jeff, ContentType::Error, Theme::Red);
  hp->append(message);
  notifier->notify(message);
  emit show(message);
}

#ifdef JEFF_WITH_QT_WIDGETS
/*! @brief Creates Message @a message_widget, inserts a widget into it and displays a message. */
void Core::got_modal(ModalHandler *m_handler) {
  MessageData message = get_message(m_handler->getPrisoner()->objectName(),
                                    Author::Jeff, ContentType::Widget, Theme::Std);
  emit show_modal(message, m_handler);
}
#endif

/*! @brief Creates @a message. */
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

/*! @brief Sets the status of the monologue mode. */
void Core::set_monologue_enabled(const bool enabled) {
  monologue_enabled = enabled;
#ifdef JEFF_WITH_QT_WIDGETS
  emit changeMenuBarMonologueCheckbox(enabled);
#endif
}

/*! @brief Sends a greeting on behalf of the user, if the corresponding setting is enabled. */
void Core::start() {
  sem->startup();
  if ((*basis)[basis->isHistoryKeepingEnabledSt].toBool())
    hp->load();
  if (not (*basis)[basis->customScannerSt].toString().isEmpty()) {
    custom_scanner = new CustomScanScript();
    custom_scanner->path = (*basis)[basis->customScannerSt].toString();
    custom_scanner->fn_name = (*basis)[basis->customScannerFnSt].toString();
    jck->set_custom_scanner(custom_scanner);
  }
  if (not (*basis)[basis->customComposerSt].toString().isEmpty()) {
    custom_composer = new CustomComposeScript();
    custom_composer->path = (*basis)[basis->customComposerSt].toString();
    custom_composer->fn_name = (*basis)[basis->customComposerFnSt].toString();
    custom_composer->send_adprops = (*basis)[basis->customComposerSASt].toBool();
    jck->set_custom_composer(custom_composer);
  }
  if ((*basis)[basis->isGreetingsEnabledSt].toBool())
    got_message_from_user((*basis)[basis->greetingsMsg].toString());
}
