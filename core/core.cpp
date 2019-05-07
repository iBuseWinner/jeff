#include "core.h"

core::core(QObject *p) : QObject(p) {
  QSqlDatabase::addDatabase("QSQLITE");
  connect(st, &settings::jsonError, this, &core::getError);
  connect(st, &settings::roWarning, this, &core::getWarning);
  connect(sq, &sqlite::sqliteError, this, &core::getError);
  connect(sq, &sqlite::sqliteWarning, this, &core::getWarning);
  connect(stdTs, &standardTemplates::showASWDialog, this, &core::getWidget);
  connect(nlp, &NLPmodule::ready, this, &core::getNLP);
}

void core::getUser(QString _ue) {
  if (_ue == QString()) return;
  emit show(new AMessage(d(_ue, A::User, C::Markdown, T::Std)));
  if (stdTs->dialogues(_ue)) return;
  nlp->search(_ue);
}

void core::getNLP(QString _cn) {
  emit show(new AMessage(d(_cn, A::ASW, C::Markdown, T::Std)));
}

void core::getWarning(QString _wt) {
  emit show(new AMessage(d(_wt, A::ASW, C::Warning, T::Yellow)));
}

void core::getError(QString _et) {
  emit show(new AMessage(d(_et, A::ASW, C::Error, T::Red)));
}

void core::getWidget(QWidget *_w) {
  auto *msg = new AMessage(d(_w->objectName(), A::ASW, C::Widget, T::Std));
  _w->setParent(msg);
  msg->setWidget(_w);
  emit show(msg);
}

message core::d(QString _cn, A _a, C _ct, T _t) {
  message _d;
  _d.cn = _cn;
  _d.dt = curDT();
  _d.aType = _a;
  _d.cType = _ct;
  _d.tType = _t;
  return _d;
}
