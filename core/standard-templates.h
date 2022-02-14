#ifndef STANDARDTEMPLATES_H
#define STANDARDTEMPLATES_H

#include "core/basis.h"
#ifdef JEFF_WITH_QT_WIDGETS
#include "dialogues/about.h"
#include "dialogues/phrase-editor.h"
#include "dialogues/firststart.h"
#include "dialogues/help.h"
#include "dialogues/modal-handler.h"
#include "dialogues/settings.h"
#include "dialogues/sources.h"
#endif
#include "core/history-processor.h"
#include "core/standard-templates.h"
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
  StandardTemplates(Basis *_basis, HProcessor *_hp, QObject *parent = nullptr)
    : QObject(parent), basis(_basis), hp(_hp) {}

  // Constants:
#ifdef JEFF_WITH_QT_WIDGETS
  static inline const QString source_manager_cmd = "/sourcemanager";
  static inline const QString first_start_cmd = "/firststart";
  static inline const QString about_cmd = "/about";
  static inline const QString settings_cmd = "/settings";
  static inline const QString phrase_editor_cmd = "/phraseeditor";
#endif
  static inline const QString fast_append_cmd = "/+ ";
  static inline const QString monologue_mode_cmd = "/mm";

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

private:
  // Objects:
  Basis *basis = nullptr;
  HProcessor *hp = nullptr;
};

#endif
