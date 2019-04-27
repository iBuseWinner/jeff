#ifndef STANDARDTEMPLATES_H
#define STANDARDTEMPLATES_H

#include <QString>
#include "dialogues/about.h"
#include "dialogues/containers.h"
#include "dialogues/createcontainer.h"
#include "dialogues/firststart.h"
#include "dialogues/help.h"
#include "widgets/a_message.h"

class standardTemplates {
 public:
  QString dialogues(QString ue, AMessage *msg);
};

#endif  // STANDARDTEMPLATES_H
