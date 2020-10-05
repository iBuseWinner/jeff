#ifndef STANDARDTEMPLATES_H
#define STANDARDTEMPLATES_H

#include "core/basis.h"
#include "dialogues/about.h"
#include "dialogues/firststart.h"
#include "dialogues/help.h"
#include "dialogues/modal-handler.h"
#include "dialogues/settings.h"
#include "dialogues/sources.h"
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
   * @param[in,out] parent QObject parent
   */
  StandardTemplates(Basis *_basis, QObject *parent = nullptr)
      : QObject(parent), basis(_basis) {}

  // Functions described in 'standard-templates.cpp':
  bool dialogues(const QString &expression);
  bool fastCommands(const QString &expression);
  static QPair<QString, QStringList> inlineCommands(const QString &expression);

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

  // Constants:
  inline static const QString current_time_tmpl = "<stdt::currtime>";
};

#endif // STANDARDTEMPLATES_H
