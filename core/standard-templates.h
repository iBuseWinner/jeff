#ifndef STANDARDTEMPLATES_H
#define STANDARDTEMPLATES_H

#include <QString>
#include "core/settings.h"
#include "dialogues/about.h"
#include "dialogues/containers.h"
#include "dialogues/firststart.h"
#include "dialogues/help.h"

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
  standardTemplates(settings *_settings, QObject *parent = nullptr)
      : QObject(parent) {
    st = _settings;
  }

 signals:
  QWidget *showASWDialog(QWidget *widget);

 private:
  // Objects:
  settings *st = nullptr;
};

#endif  // STANDARDTEMPLATES_H
