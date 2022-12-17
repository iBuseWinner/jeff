#ifndef STANDARDTEMPLATES_H
#define STANDARDTEMPLATES_H

#include "core/basis.h"
#ifdef JEFF_WITH_QT_WIDGETS
#include "dialogues/about.h"
#include "dialogues/add-script.h"
#include "dialogues/phrase-editor.h"
#include "dialogues/firststart.h"
#include "dialogues/help.h"
#include "dialogues/modal-handler.h"
#include "dialogues/settings.h"
#include "dialogues/sources.h"
#endif
#include "core/history-processor.h"
#include "core/script-engine-module.h"
#include <QString>

/*!
 * @class StandardTemplates
 * @brief Provides some kinds of widgets by command.
 */
class StandardTemplates : public QObject {
  Q_OBJECT
public:
  // Functions:
  /*! @brief The constructor. */
  StandardTemplates(Basis *_basis, HProcessor *_hp, SEModule *_sem, QObject *parent = nullptr)
    : QObject(parent), basis(_basis), hp(_hp), sem(_sem) {}

  // Functions described in `standard-templates.cpp`:
#ifdef JEFF_WITH_QT_WIDGETS
  bool dialogues(const QString &expression);
#endif
  bool fast_commands(const QString &expression);

signals:
#ifdef JEFF_WITH_QT_WIDGETS
  /*! @brief Sends a ModalHandler when the widget needs to be shown on the screen. */
  ModalHandler *showModalWidget(ModalHandler *handler);
#endif
  /*! @brief Sends a signal to turn on/off monologue mode. */
  void changeMonologueMode();
  /*! @brief Notifies that the scenario has finished running. @sa ScenarioScript */
  void shutdown_scenario();

private:
  // Objects:
  Basis *basis = nullptr;
  HProcessor *hp = nullptr;
  SEModule *sem = nullptr;
};

#endif
