#include "standardtemplates.h"

QString standardTemplates::dialogues(QString ue, AMessage *msg) {
  if (ue == "/about") {
    auto *aw = new About(msg);
    msg->setMessageType(AMessage::Widget, aw);
    return QString();
  }
  if (ue == "/cm") {
    auto *cm = new Containers(msg);
    msg->setMessageType(AMessage::Widget, cm);
    return QString();
  }
  if (ue == "/first") {
    auto *fs = new FirstStart(msg);
    msg->setMessageType(AMessage::Widget, fs);
    return QString();
  }
  return ue;
}
