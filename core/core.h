#ifndef CORE_H
#define CORE_H

#include "core/basis.h"
#include "core/history-processor.h"
#include "core/nlp-module.h"
#include "core/python-module.h"
#include "core/standard-templates.h"
#include "dialogues/modal-handler.h"
#include "core/database/sqlite.h"
#include <QObject>
#include <QTimer>

/*!
 * @class Core
 * @brief Controls I/O.
 * @details Manages objects associated with receiving messages from different
 * sources.
 * @sa Basis, HProcessor, NLPmodule, StdTemplates, Message
 */
class Core : public QObject {
  Q_OBJECT
public:
  // Objects:
  Basis *basis = new Basis();
  HProcessor *history_processor = new HProcessor(basis);
  PythonModule *pm = new PythonModule(history_processor, basis);

  // Functions described in `core.cpp`:
  Core(QObject *parent = nullptr);
  ~Core();
  void got_message_from_user(const QString &user_expression);
  void got_message_from_nlp(const QString &result_expression);
  void got_message_from_script(const QString &message);
  void got_warning(const QString &warning_text);
  void got_error(const QString &error_text);
  void got_modal(ModalHandler *m_handler);
  void show_history(Messages message_history);
  void set_monologue_enabled(const bool enabled);
  MessageData get_message(const QString &content, Author author, 
                          ContentType content_type, Theme theme);

signals:
#ifdef QT
  /*!
   * @brief Sends a message to Display.
   * @sa Message, Display
   */
  ModalHandler *show_modal(MessageData message_data, ModalHandler *handler);
#endif
  
  /*!
   * @brief Sends a message.
   * @sa MessageData
   */
  MessageData show(MessageData message_data);

  /*!
   * @brief Sets the monologue mode.
   */
  bool changeMenuBarMonologueCheckbox(bool enabled);

private:
  Q_DISABLE_COPY(Core)

  // Objects:
  NLPmodule *_nlp = new NLPmodule(basis, pm);
  StandardTemplates *_standard_templates = new StandardTemplates(basis, history_processor);
  
  bool _monologue_enabled = false;
};

#endif
