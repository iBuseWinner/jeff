#include "standard-templates.h"

standardTemplates::standardTemplates(QObject *p) : QObject(p) {}

bool standardTemplates::dialogues(const QString &_cn) {
  if (_cn == "/about") {
    emit showASWDialog(new About());
    return true;
  }
  if (_cn == "/cm") {
    emit showASWDialog(new Containers());
    return true;
  }
  if (_cn == "/first") {
    emit showASWDialog(new FirstStart());
    return true;
  }
  return false;
}
