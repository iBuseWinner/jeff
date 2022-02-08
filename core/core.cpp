#include "core.h"

/*! @brief The constructor. */
Core::Core(QObject *parent) : QObject(parent) {
  connect(basis, &Basis::settings_warning, this, &Core::got_warning);
  connect(basis, &Basis::send, this, &Core::got_message_from_script);
  connect(basis, &Basis::search_again, this, &Core::got_message_to_search_again);
  connect(basis, &Basis::send_as_user, this, &Core::got_message_from_script_as_user);
  connect(basis, &Basis::send_status, this, &Core::got_status_from_script);
  basis->check_settings_file();
  basis->check_default_source();
  connect(server, &Server::server_error, this, &Core::got_error);
  connect(pm, &PythonModule::script_exception, this, &Core::got_warning);
  connect(pm, &PythonModule::send, this, &Core::got_message_from_script);
  connect(basis->sql, &SQLite::sqlite_error, this, &Core::got_error);
  connect(std_templates, &StandardTemplates::showModalWidget, this, &Core::got_modal);
  connect(nlp, &NLPmodule::response, this, &Core::got_message_from_nlp);
  connect(std_templates, &StandardTemplates::changeMonologueMode, this,
          [this] { set_monologue_enabled(not monologue_enabled); });
  set_monologue_enabled((*basis)[basis->isMonologueEnabledSt].toBool());
  server->start();
}

/*! @brief The destructor. */
Core::~Core() {
  delete nlp;
  delete pm;
  if ((*basis)[basis->isHistoryKeepingEnabledSt].toBool()) hp->save();
  delete hp;
  delete std_templates;
  server->stop();
  delete server;
  delete basis;
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
  /*! If a user has entered the command, there is no need to run other modules. */
  if (std_templates->dialogues(user_expression)) return;
  if (std_templates->fast_commands(user_expression)) return;
  nlp->search_for_suggests(user_expression);
}

/*! @brief Processes the output of the NLP module @a result_expression and
 *  displays a message on the screen.  */
void Core::got_message_from_nlp(const QString &result_expression) {
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
        emit show(mdata);
        /*! Search again if monologue mode enabled. */
        if (monologue_enabled) nlp->search_for_suggests(result_expression);
      });
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
        emit show(mdata);
        /*! Search again if monologue mode enabled. */
        if (monologue_enabled) nlp->search_for_suggests(message);
      });
}

/*! @brief Searches again. */
void Core::got_message_to_search_again(const QString &rephrased_message) {
  if (rephrased_message.isEmpty()) return;
  if (std_templates->dialogues(rephrased_message)) return;
  if (std_templates->fast_commands(rephrased_message)) return;
  nlp->search_for_suggests(rephrased_message);
}

/*! @brief Shows the message and searches again. */
void Core::got_message_from_script_as_user(const QString &message) {
  /*! Does not respond to blank input. */
  if (message.isEmpty()) return;
  /*! Displays the entered message on the screen. */
  MessageData mdata = get_message(message, Author::Jeff, ContentType::Markdown, Theme::Blue);
  hp->append(mdata);
  emit show(mdata);
  /*! If a user has entered the command, there is no need to run other modules. */
  if (std_templates->dialogues(message)) return;
  if (std_templates->fast_commands(message)) return;
  nlp->search_for_suggests(message);
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
        hp->append(mdata);
        QPair<QString, MessageData> id_and_message_data(id_and_message.first, mdata);
        emit show_status(id_and_message_data);
      });
}

/*! @brief Displays @a warning_text. */
void Core::got_warning(const QString &warning_text) {
  /*! The warning color is yellow. */
  MessageData message = get_message(warning_text, Author::Jeff, ContentType::Warning, Theme::Yellow);
  hp->append(message);
  emit show(message);
}

/*! @brief Displays @a errorText. */
void Core::got_error(const QString &error_text) {
  /*! The error color is red. */
  MessageData message = get_message(error_text, Author::Jeff, ContentType::Error, Theme::Red);
  hp->append(message);
  emit show(message);
}

/*! @brief Creates Message @a message_widget, inserts a widget into it and displays a message. */
void Core::got_modal(ModalHandler *m_handler) {
  MessageData message = get_message(m_handler->getPrisoner()->objectName(),
                                    Author::Jeff, ContentType::Widget, Theme::Std);
  hp->append(message);
  emit show_modal(message, m_handler);
}

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
  emit changeMenuBarMonologueCheckbox(enabled);
}

/*! @brief Sends a greeting on behalf of the user, if the corresponding setting is enabled. */
void Core::start() {
  if ((*basis)[basis->isHistoryKeepingEnabledSt].toBool())
    hp->load();
  if ((*basis)[basis->isGreetingsEnabledSt].toBool())
    got_message_from_user((*basis)[basis->greetingsMsg].toString());
}
