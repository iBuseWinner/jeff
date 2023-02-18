#include "core.hpp"

/*! @brief The constructor. */
Core::Core(QObject *parent) : QObject(parent) {
  connect(basis, &Basis::info, this, &Core::got_info);
  connect(basis, &Basis::warn, this, &Core::got_warning);
  connect(basis, &Basis::send, this, &Core::got_message_from_script);
  connect(basis, &Basis::search_again, this, &Core::got_message_to_search_again);
  connect(basis, &Basis::send_as_user, this, &Core::got_message_from_script_as_user);
  connect(basis, &Basis::send_status, this, &Core::got_status_from_script);
  connect(basis, &Basis::start_scenario, this, &Core::got_scenario_start);
  connect(basis, &Basis::schedule_scenario, this, &Core::add_to_scheduler);
  connect(basis, &Basis::shutdown_scenario, this, &Core::got_scenario_shutting);
  basis->check_settings_file();
  basis->check_default_source();
  connect(server, &Server::server_error, this, &Core::got_error);
  connect(em, &ExtensionsManager::extension_exception, this, &Core::got_warning);
  connect(basis->sql, &SQLite::sqlite_error, this, &Core::got_error);
#ifdef JEFF_WITH_QT_WIDGETS
  connect(std_templates, &StandardTemplates::showModalWidget, this, &Core::got_modal);
#endif
  connect(jck, &JCKController::empty, this, &Core::got_no_jck_output);
  connect(jck, &JCKController::response, this, &Core::got_message_from_jck);
  connect(jck, &JCKController::start_extension, this, &Core::got_extension_start);
  connect(std_templates, &StandardTemplates::shutdown_scenario, this, &Core::got_scenario_shutting);
  connect(std_templates, &StandardTemplates::changeMonologueMode, this,
          [this] { set_monologue_enabled(not monologue_enabled); });
  connect(std_templates, &StandardTemplates::show_info, this, &Core::got_info);
  connect(std_templates, &StandardTemplates::send_to_extension, this, &Core::got_to_extension);
  set_monologue_enabled((*basis)[basis->isMonologueEnabledSt].toBool());
  quint16 port = quint16((*basis)[basis->serverPortSt].toInt());
  if (port == 0) port = 8005;
  server->start(QHostAddress::LocalHost, port);
  Yellog::Trace("All core things are connected. Waiting for Core::start()...");
}

/*! @brief Sends a greeting on behalf of the user, if the corresponding setting is enabled. */
void Core::start() {
  em->startup();
  if ((*basis)[basis->isHistoryKeepingEnabledSt].toBool()) hp->load();
  if (not (*basis)[basis->customScannerSt].toString().isEmpty()) {
    custom_scanner = ScriptMeta::from_string((*basis)[basis->customScannerSt].toString());
    jck->set_custom_scanner(custom_scanner);
  }
  if (not (*basis)[basis->customComposerSt].toString().isEmpty()) {
    custom_composer = ScriptMeta::from_string((*basis)[basis->customComposerSt].toString());
    jck->set_custom_composer(custom_composer);
  }
  if ((*basis)[basis->isGreetingsEnabledSt].toBool()) got_message_from_user((*basis)[basis->greetingsMsg].toString());
  Yellog::Trace("All core things are definitely done. Have a nice day!");
}

/*! @brief The destructor. */
Core::~Core() {
  Yellog::Trace("After this message all core things will be definitely GONE.");
  Yellog::Trace("And again, have a nice day!");
  delete jck;
  delete em;
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

/*! @brief Jeff does not respond to blank input. */
bool Core::fits(const QString &input) { if (input.isEmpty()) return false; return true; }

/*! @brief TBD */
bool Core::check(const QString &input) {
#ifdef JEFF_WITH_QT_WIDGETS
  if (std_templates->dialogues(input)) return true;
#endif
  if (std_templates->fast_commands(input)) return true;
  return false;
}

/*! @brief TBD */
int Core::time_bounds() {
  auto is_bounded = (*basis)[basis->isDelayEnabledSt].toBool();
  if (is_bounded) {
    auto b1 = (*basis)[basis->minDelaySt].toInt();
    auto b2 = (*basis)[basis->maxDelaySt].toInt();
    return QRandomGenerator().bounded(b1, b2);
  } else return 0;
}

/*! @brief Handles input @a user_expression, displays a message on the screen and launches modules. */
void Core::got_message_from_user(const QString &user_expression) {
  if (not fits(user_expression)) return;
  auto *message = create_message(user_expression, Author::User, ContentType::Markdown, Theme::Std);
  hp->append(message);
  emit show(message);
  if (check(user_expression)) return;
  notifier->notify(message);
  if (is_scenario_running) return;
  jck->search_for_suggests(user_expression);
}

/*! @brief Processes the output of the JCK module @a result_expression and displays a message on the screen. */
void Core::got_message_from_jck(const QString &result_expression) {
  if (not fits(result_expression)) return;
  QTimer::singleShot(time_bounds(), this, [this, result_expression] {
    auto *message = create_message(result_expression, Author::Jeff, ContentType::Markdown, Theme::Std);
    hp->append(message);
    emit show(message);
    if (check(result_expression)) return;
    notifier->notify(message);
    if (monologue_enabled) jck->search_for_suggests(result_expression);
  });
}

/*! @brief Notifies those extensions that are designed to handle cases where JCK cannot answer a question. */
void Core::got_no_jck_output(const QString &user_expression) {
  auto *message = create_message(user_expression, Author::User, ContentType::Markdown, Theme::Std);
  notifier->notify(message, true);
}

/*! @brief Runs an extension process and sets notifications if necessary. */
void Core::got_extension_start(ExtensionMeta *extension_meta) {
  em->add_extension(extension_meta);
}

/*! @brief Notifies the extension that the scenario has started and passes the authentication data. */
void Core::got_scenario_start(const ScenarioServerMeta &scenario_meta) {
  is_scenario_running = true;
  current_scenario = scenario_meta;
  notifier->set_scenario(current_scenario);
  notifier->notify_scenario_first_time(current_scenario.auth_key);
  emit change_menubar_scenario_name(current_scenario.name);
}

/*! @brief TBD */
void Core::add_to_scheduler(const ScenarioServerMeta &scenario_meta) {
  _squeue.append(scenario_meta);
  notifier->notify_about_queued(scenario_meta);
}

/*! @brief Disables scenario. */
void Core::got_scenario_shutting() {
  notifier->finish_scenario();
  basis->clear_stoken();
  is_scenario_running = false;
  emit change_menubar_scenario_name(QString());
  if (not _squeue.isEmpty()) {
    auto scenario_meta = _squeue.takeFirst();
    basis->set_stoken(scenario_meta.auth_key);
    got_scenario_start(scenario_meta);
  }
}

/*! @brief Shows output from script or outter app on the screen. */
void Core::got_message_from_script(const QString &outter_message) {
  if (not fits(outter_message)) return;
  QTimer::singleShot(time_bounds(), this, [this, outter_message] {
    auto *message = create_message(outter_message, Author::Jeff, ContentType::Markdown, Theme::Std);
    hp->append(message);
    emit show(message);
    if (check(outter_message)) return;
    notifier->notify(message);
    if (monologue_enabled) jck->search_for_suggests(outter_message);
  });
}

/*! @brief Searches again. */
void Core::got_message_to_search_again(const QString &rephrased_message) {
  if (not fits(rephrased_message)) return;
  if (check(rephrased_message)) return;
  jck->search_for_suggests(rephrased_message);
}

/*! @brief Shows the message and searches again. */
void Core::got_message_from_script_as_user(const QString &outter_message) {
  if (not fits(outter_message)) return;
  auto *message = create_message(outter_message, Author::User, ContentType::Markdown, Theme::Blue);
  hp->append(message);
  emit show(message);
  if (check(outter_message)) return;
  notifier->notify(message);
  jck->search_for_suggests(outter_message);
}

/*! @brief Shows updateable message. @details Checks id's length. */
void Core::got_status_from_script(QPair<QString, QString> id_and_message) {
  if (id_and_message.first.isEmpty() or id_and_message.second.isEmpty()) return;
  if (id_and_message.first.length() < 24) return; /*!< Too much small ID; reject now. */
  auto *message = create_message(id_and_message.second, Author::Jeff, ContentType::Markdown, Theme::Std);
  QTimer::singleShot(time_bounds(), this, [this, message, id_and_message] {
    hp->append(message);
    notifier->notify(message);
    QPair<QString, MessageMeta *> id_and_message_data(id_and_message.first, message);
    emit show_status(id_and_message_data);
  });
}

/*! @brief Displays @a info_text. */
void Core::got_info(const QString &info_text) {
  if (not fits(info_text)) return;
  auto *message = create_message(info_text, Author::Jeff, ContentType::Markdown, Theme::Green);
  hp->append(message);
  emit show(message);
  notifier->notify(message);
}

/*! @brief Displays @a warning_text. */
void Core::got_warning(const QString &warning_text) {
  if (not fits(warning_text)) return;
  auto *message = create_message(warning_text, Author::Jeff, ContentType::Warning, Theme::Yellow);
  hp->append(message);
  emit show(message);
  notifier->notify(message);
}

/*! @brief Displays @a errorText. */
void Core::got_error(const QString &error_text) {
  if (not fits(error_text)) return;
  auto *message = create_message(error_text, Author::Jeff, ContentType::Error, Theme::Red);
  hp->append(message);
  emit show(message);
  notifier->notify(message);
}

/*! @brief TBD */
void Core::got_to_extension(const QString &extension_name, const QString &text) {
  auto *extension_meta = em->get_ext_meta_by_name(extension_name);
  if (not extension_meta) return;
  if (not extension_meta->is_server) return;
  auto *message = create_message(text, Author::undefA, ContentType::Text, Theme::Std);
  notifier->notify_only(extension_meta, message);
}

#ifdef JEFF_WITH_QT_WIDGETS
/*! @brief Creates Message @a message_widget, inserts a widget into it and displays a message. */
void Core::got_modal(ModalHandler *m_handler) {
  Yellog::Trace("Got a dialogue.");
  auto *message = create_message(m_handler->getPrisoner()->objectName(), Author::Jeff, ContentType::Widget, Theme::Std);
  emit show_modal(message, m_handler);
}
#endif

/*! @brief Creates @a message. */
MessageMeta *Core::create_message(const QString &content, Author author, ContentType content_type, Theme theme) {
  auto *message = new MessageMeta();
  message->content = content;
  message->author = author;
  message->content_type = content_type;
  message->theme = theme;
  return message;
}

/*! @brief Sets the status of the monologue mode. */
void Core::set_monologue_enabled(const bool enabled) {
  monologue_enabled = enabled;
#ifdef JEFF_WITH_QT_WIDGETS
  emit change_menubar_mmode(enabled);
#endif
  Yellog::Trace("Monologue mode changed to: %d", int(enabled));
}
