#ifndef STANDARDTEMPLATES_H
#define STANDARDTEMPLATES_H

#include <QString>
#include "core/core-methods.h"
#include "dialogues/about.h"
#include "dialogues/containers.h"
#include "dialogues/firststart.h"
#include "dialogues/help.h"
#include "dialogues/settings.h"

/*!
 * Class: standardTemplates
 * Provides some kinds of widgets by command.
 */
class standardTemplates : public QObject {
  Q_OBJECT
 public:
  // Functions:
  bool dialogues(const QString &expression);

  /*! Class initialization. */
  standardTemplates(CoreMethods *_Meths, QObject *parent = nullptr)
      : QObject(parent) {
    Meths = _Meths;
  }

 signals:
  QWidget *showASWDialog(QWidget *widget);

 private:
  // Objects:
  CoreMethods *Meths = nullptr;
};

#endif  // STANDARDTEMPLATES_H
