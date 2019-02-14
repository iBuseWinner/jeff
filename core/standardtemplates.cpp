#include "standardtemplates.h"

QString standardTemplates::dialogues(QString userExpression, AMessage *msg) {
  if (userExpression == "/about") {
    auto *about = new About(msg);
    msg->setMessageType(AMessage::Widget, about);
    return QString();
  } if (userExpression == "/cm") {
    auto *cm = new Containers(msg);
    msg->setMessageType(AMessage::Widget, cm);
    return QString();
  } else if (userExpression == "/first") {
    auto *fs = new FirstStart(msg);
    msg->setMessageType(AMessage::Widget, fs);
    return QString();
  }
  return userExpression;
}
