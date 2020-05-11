#ifndef STANDARDTEMPLATES_H
#define STANDARDTEMPLATES_H

#include "core/basis.h"
#include "dialogues/about.h"
#include "dialogues/sources.h"
#include "dialogues/firststart.h"
#include "dialogues/help.h"
#include "dialogues/settings.h"
#include <QString>

/*!
 * Class: StdTemplates
 * Provides some kinds of widgets by command.
 * {basis} <- core methods
 */
class StdTemplates : public QObject {
  Q_OBJECT
public:
  // Functions:
  bool dialogues(const QString &expression);
  bool fastCommands(const QString &expression);

  /*! Class initialization. */
  StdTemplates(Basis *_basis, QObject *parent = nullptr) : QObject(parent) {
    basis = _basis;
  }

signals:
  QWidget *showASWDialog(QWidget *widget);
  void changeMonologueMode();

private:
  // Objects:
  Basis *basis = nullptr;
};

#endif // STANDARDTEMPLATES_H
