#ifndef STANDARDTEMPLATES_H
#define STANDARDTEMPLATES_H

#include "core-kit/basis.h"
#ifdef JEFF_WITH_QT_WIDGETS
#include "dialogues/about.h"
#include "dialogues/phrase-editor.h"
#include "dialogues/firststart.h"
#include "dialogues/help.h"
#include "dialogues/modal-handler.h"
#include "dialogues/settings.h"
#include "dialogues/sources-editor.h"
#endif
#include "core-kit/extensions-manager.h"
#include "core-kit/history-processor.h"
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
  StandardTemplates(Basis *_basis, HProcessor *_hp, ExtensionsManager *_em, QObject *parent = nullptr)
    : QObject(parent), basis(_basis), hp(_hp), em(_em) {}

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
  ExtensionsManager *em = nullptr;
};

#endif
