#ifndef STANDARDTEMPLATES_H
#define STANDARDTEMPLATES_H

#include "core/basis.h"
#include "dialogues/about.h"
#include "dialogues/expression-editor.h"
#include "dialogues/firststart.h"
#include "dialogues/help.h"
#include "dialogues/modal-handler.h"
#include "dialogues/settings.h"
#include "dialogues/sources.h"
#include "core/history-processor.h"
#include "core/standard-templates.h"
#include <QString>
#include <QTime>

/*!
 * @class StandardTemplates
 * @brief Provides some kinds of widgets by command.
 */
class StandardTemplates : public QObject {
  Q_OBJECT
public:
  // Functions:
  /*!
   * @fn StandardTemplates::StandardTemplates
   * @brief The constructor.
   * @param[in,out] _basis reference to the Basis instance
   * @param[in,out] _hp reference to the HProcessor instance
   * @param[in,out] parent QObject parent
   */
  StandardTemplates(Basis *_basis, HProcessor *_hp, QObject *parent = nullptr)
    : QObject(parent), basis(_basis), hp(_hp) {}

  // Constants:
  static inline const QString source_manager_cmd = "/sourcemanager";
  static inline const QString first_start_cmd = "/firststart";
  static inline const QString about_cmd = "/about";
  static inline const QString settings_cmd = "/settings";
  static inline const QString fast_append_cmd = "/+ ";
  static inline const QString expression_editor_cmd = "/expressioneditor";
  static inline const QString monologue_mode_cmd = "/mm";

  // Functions described in `standard-templates.cpp`:
  bool dialogues(const QString &expression);
  bool fast_commands(const QString &expression);
  static QPair<QString, QStringList> inline_commands(const QString &expression);

signals:
  /*!
   * @brief Sends a ModalHandler when the widget needs to be shown on the
   * screen.
   */
  ModalHandler *showModalWidget(ModalHandler *handler);

  /*!
   * @brief Sends a signal to turn on/off monologue mode.
   */
  void changeMonologueMode();

private:
  // Objects:
  Basis *basis = nullptr;
  HProcessor *hp = nullptr;
};

#endif
