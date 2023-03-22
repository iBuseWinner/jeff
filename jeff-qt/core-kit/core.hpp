#ifndef CORE_H
#define CORE_H

#include "core-kit/basis.hpp"
#include "core-kit/history-processor.hpp"
#include "core-kit/jeff-core-kit.hpp"
#include "core-kit/local-server.hpp"
#include "core-kit/notify-client.hpp"
#include "core-kit/extensions-manager.hpp"
#include "core-kit/standard-templates.hpp"
#include "core-kit/extensions/extension.hpp"
#include "core-kit/extensions/scenario.hpp"
#include "dialogues/modal-handler.hpp"
#include <QObject>
#include <QPair>
#include <QTimer>

/*! @class Core
 *  @brief Controls I/O.
 *  @details Manages objects associated with receiving messages from different sources.
 *  @sa Basis, HProcessor, JCK && JCKController, StandardTemplates, MessageMeta  */
class Core : public QObject {
  Q_OBJECT
  Q_DISABLE_COPY(Core)
public:
  // Objects:
  Basis *basis = new Basis();
  HProcessor *hp = new HProcessor(basis);
  class Server *server = new class Server(basis);
  NotifyClient *notifier = new NotifyClient();
  ExtensionsManager *em = new ExtensionsManager(hp, basis, notifier);

  // Functions described in `core.cpp`:
  Core(QObject *parent = nullptr);
  void start();
  ~Core();
  void got_message_from_user(const QString &user_expression);
  void got_message_from_jck(const QString &result_expression);
  void got_no_jck_output(const QString &user_expression);
  void got_extension_start(ExtensionMeta *extension_meta);
  void got_scenario_start(const ScenarioServerMeta &scenario_meta);
  void add_to_scheduler(const ScenarioServerMeta &scenario_meta);
  void got_scenario_shutting();
  void notify_scenario_first_time();
  void got_message_from_script(const QString &outter_message);
  void got_message_to_search_again(const QString &rephrased_message);
  void got_message_from_script_as_user(const QString &outter_message);
  void got_status_from_script(QPair<QString, QString> id_and_message);
  void got_info(const QString &info_text);
  void got_warning(const QString &warning_text);
  void got_error(const QString &error_text);
  void got_to_extension(const QString &extension_name, const QString &text);
#ifdef JEFF_WITH_QT_WIDGETS
  void got_modal(ModalHandler *m_handler);
#endif
  void set_monologue_enabled(const bool enabled);
  MessageMeta *create_message(const QString &content, Author author, ContentType content_type, Theme theme);

signals:
#ifdef JEFF_WITH_QT_WIDGETS
  /*! @brief Sends a message to Display. */
  ModalHandler *show_modal(MessageMeta *message_data, ModalHandler *handler);
  /*! @brief Sets the monologue mode in MenuBar. */
  bool change_menubar_mmode(bool enabled);
#endif
  /*! @brief Sends a message. */
  MessageMeta *show(MessageMeta *message_data);
  /*! @brief Sends an updateable message. */
  QPair<QString, MessageMeta *> show_status(QPair<QString, MessageMeta *> id_and_message_data);
  /*! @brief Asks the Menubar to set a new script name (it is displayed in the main window). */
  QString change_menubar_scenario_name(QString scenario_name);

private:
  // Objects:
  bool monologue_enabled = false;
  JCKController *jck = new JCKController(basis, hp);
  StandardTemplates *std_templates = new StandardTemplates(basis, hp, em);
  bool is_scenario_running = false;
  bool is_user_input_already_sent = false;
  ScenarioServerMeta current_scenario;
  Scenarios _squeue = Scenarios(); /*!< Scenarios queue. */
  
  // Functions described in `core.cpp`:
  bool fits(const QString &input);
  bool check(const QString &input);
  int time_bounds();
};

#endif
