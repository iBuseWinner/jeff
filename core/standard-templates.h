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

/*!
 * Class: StdTemplates.
 * Provides some kinds of widgets by command.
 */
class StdTemplates : public QObject {
  Q_OBJECT
public:
  // Functions:
  bool dialogues(const QString &expression);
  bool fastCommands(const QString &expression);

  /*! Class initialization. */
  StdTemplates(Basis *_basis, QObject *parent = nullptr)
      : QObject(parent), basis(_basis) {}

signals:
  ModalHandler *showModalWidget(ModalHandler *handler);
  void changeMonologueMode();

private:
  // Objects:
  Basis *basis = nullptr;
};

#endif // STANDARDTEMPLATES_H
