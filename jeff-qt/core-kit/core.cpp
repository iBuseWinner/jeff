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

/*! @brief Handles input @a user_expression, displays a message on the screen and launches modules. */
void Core::got_message_from_user(const QString &user_expression) {
  /*! Does not respond to blank input. */
  if (user_expression.isEmpty()) return;
  /*! Displays the entered message on the screen. */
  Yellog::Trace("Got a message from user.");
  auto *message = get_message(user_expression, Author::User, ContentType::Markdown, Theme::Std);
  hp->append(message);
  emit show(message);
  /*! If user has entered any command, there is no need to run other modules. */
#ifdef JEFF_WITH_QT_WIDGETS
  if (std_templates->dialogues(user_expression)) return;
#endif
  if (std_templates->fast_commands(user_expression)) return;
  Yellog::Trace("\tThe message is neither dialogue nor fast command.");
  notifier->notify(message); /*!< Notifies only when message isn't any dialog or command. */
  if (is_scenario_running) return; /*!< @sa ScenarioScript */
  Yellog::Trace("\tNo scenario, searching for suggests.");
  jck->search_for_suggests(user_expression);
}

/*! @brief Processes the output of the JCK module @a result_expression and displays a message on the screen. */
void Core::got_message_from_jck(const QString &result_expression) {
  if (result_expression.isEmpty()) return;
  Yellog::Trace("Got a message from JCK.");
  auto *message = get_message(result_expression, Author::Jeff, ContentType::Markdown, Theme::Std);
  /*! Delay is triggered if enabled. */
  Yellog::Trace("\tSetting delay and sending message...");
  QTimer::singleShot(
      (*basis)[basis->isDelayEnabledSt].toBool()
          ? QRandomGenerator().bounded((*basis)[basis->minDelaySt].toInt(), (*basis)[basis->maxDelaySt].toInt())
          : 0,
      this, [this, message, result_expression] {
        hp->append(message);
        notifier->notify(message);
        emit show(message);
        /*! Search again if monologue mode enabled. */
        if (monologue_enabled) jck->search_for_suggests(result_expression);
      });
}

/*! @brief Notifies those extensions that are designed to handle cases where JCK cannot answer a question. */
void Core::got_no_jck_output(const QString &user_expression) {
  Yellog::Trace("Got no output from JCK. Gonna notify all others...");
  auto *message = get_message(user_expression, Author::User, ContentType::Markdown, Theme::Std);
  notifier->notify(message, true);
}

/*! @brief Runs an extension process and sets notifications if necessary. */
void Core::got_extension_start(ExtensionMeta *extension_meta) {
  em->add_extension(extension_meta);
}

/*! @brief Notifies the extension that the scenario has started and passes the authentication data. */
void Core::got_scenario_start(ScenarioServerMeta scenario_meta) {
  Yellog::Trace("Got scenario start. Notifier goes brrr... (it's okay, don't worry.)");
  is_scenario_running = true;
  current_scenario = scenario_meta;
  notifier->set_scenario(current_scenario);
  notifier->notify_scenario_first_time(current_scenario.auth_key);
  emit change_menubar_scenario_name(current_scenario.name);
}

/*! @brief Disables scenario. */
void Core::got_scenario_shutting() {
  notifier->finish_scenario();
  basis->clear_stoken();
  is_scenario_running = false;
  emit change_menubar_scenario_name(QString());
  Yellog::Trace("Scenario is finished.");
}

/*! @brief Shows output from script or outter app on the screen. */
void Core::got_message_from_script(const QString &outter_message) {
  if (outter_message.isEmpty()) return;
  Yellog::Trace("Got a message from script.");
  auto *message = get_message(outter_message, Author::Jeff, ContentType::Markdown, Theme::Std);
  /*! Delay is triggered if enabled. */
  Yellog::Trace("\tSetting delay and sending message...");
  QTimer::singleShot(
      (*basis)[basis->isDelayEnabledSt].toBool()
          ? QRandomGenerator().bounded((*basis)[basis->minDelaySt].toInt(), (*basis)[basis->maxDelaySt].toInt())
          : 0,
      this, [this, message, outter_message] {
        hp->append(message);
        notifier->notify(message);
        emit show(message);
        /*! Search again if monologue mode enabled. */
        if (monologue_enabled) jck->search_for_suggests(outter_message);
      });
}

/*! @brief Searches again. */
void Core::got_message_to_search_again(const QString &rephrased_message) {
  if (rephrased_message.isEmpty()) return;
  Yellog::Trace("Got a message to search again.");
#ifdef JEFF_WITH_QT_WIDGETS
  if (std_templates->dialogues(rephrased_message)) return;
#endif
  if (std_templates->fast_commands(rephrased_message)) return;
  jck->search_for_suggests(rephrased_message);
}

/*! @brief Shows the message and searches again. */
void Core::got_message_from_script_as_user(const QString &outter_message) {
  /*! Does not respond to blank input. */
  if (outter_message.isEmpty()) return;
  /*! Displays the entered message on the screen. */
  Yellog::Trace("Got a message from script to be sent as from user.");
  auto *message = get_message(outter_message, Author::User, ContentType::Markdown, Theme::Blue);
  hp->append(message);
  notifier->notify(message);
  emit show(message);
  /*! If a user has entered the command, there is no need to run other modules. */
#ifdef JEFF_WITH_QT_WIDGETS
  if (std_templates->dialogues(outter_message)) return;
#endif
  if (std_templates->fast_commands(outter_message)) return;
  Yellog::Trace("\tThe message is neither dialogue nor fast command.");
  if (is_scenario_running) return; /*!< @sa ScenarioScript */
  Yellog::Trace("\tNo scenario, searching for suggests.");
  jck->search_for_suggests(outter_message);
}

/*! @brief Shows updateable message. @details Checks id's length. */
void Core::got_status_from_script(QPair<QString, QString> id_and_message) {
  if (id_and_message.first.isEmpty() or id_and_message.second.isEmpty()) return;
  if (id_and_message.first.length() < 24) return;
  Yellog::Trace("Hello darkness, my old friend, I've come to talk with you again... and again... ");
  Yellog::Trace("You've just got an updateable message though.");
  auto *message = get_message(id_and_message.second, Author::Jeff, ContentType::Markdown, Theme::Std);
  /*! Delay is triggered if enabled. */
  Yellog::Trace("\tSetting delay and sending message...");
  QTimer::singleShot(
      (*basis)[basis->isDelayEnabledSt].toBool()
          ? QRandomGenerator().bounded((*basis)[basis->minDelaySt].toInt(), (*basis)[basis->maxDelaySt].toInt())
          : 0,
      this, [this, message, id_and_message] {
        notifier->notify(message);
        QPair<QString, MessageMeta *> id_and_message_data(id_and_message.first, message);
        emit show_status(id_and_message_data);
      });
}

/*! @brief Displays @a info_text. */
void Core::got_info(const QString &info_text) {
  /*! The warning color is green. */
  Yellog::Trace("Got an info.");
  auto *message = get_message(info_text, Author::Jeff, ContentType::Markdown, Theme::Green);
  hp->append(message);
  notifier->notify(message);
  emit show(message);
}

/*! @brief Displays @a warning_text. */
void Core::got_warning(const QString &warning_text) {
  /*! The warning color is yellow. */
  Yellog::Trace("Got a warning.");
  auto *message = get_message(warning_text, Author::Jeff, ContentType::Warning, Theme::Yellow);
  hp->append(message);
  notifier->notify(message);
  emit show(message);
}

/*! @brief Displays @a errorText. */
void Core::got_error(const QString &error_text) {
  /*! The error color is red. */
  Yellog::Trace("Got an error.");
  auto *message = get_message(error_text, Author::Jeff, ContentType::Error, Theme::Red);
  hp->append(message);
  notifier->notify(message);
  emit show(message);
}

#ifdef JEFF_WITH_QT_WIDGETS
/*! @brief Creates Message @a message_widget, inserts a widget into it and displays a message. */
void Core::got_modal(ModalHandler *m_handler) {
  Yellog::Trace("Got a dialogue.");
  auto *message = get_message(m_handler->getPrisoner()->objectName(), Author::Jeff, ContentType::Widget, Theme::Std);
  emit show_modal(message, m_handler);
}
#endif

/*! @brief Creates @a message. */
MessageMeta *Core::get_message(const QString &content, Author author, ContentType content_type, Theme theme) {
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
