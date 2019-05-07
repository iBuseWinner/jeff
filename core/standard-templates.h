#ifndef STANDARDTEMPLATES_H
#define STANDARDTEMPLATES_H

#include <QString>
#include "dialogues/about.h"
#include "dialogues/containers.h"
#include "dialogues/createcontainer.h"
#include "dialogues/firststart.h"
#include "dialogues/help.h"

class standardTemplates : public QObject {
  Q_OBJECT
 public:
  standardTemplates(QObject *p = nullptr);
  bool dialogues(const QString &_cn);

 signals:
  QWidget *showASWDialog(QWidget *w);
};

#endif  // STANDARDTEMPLATES_H
