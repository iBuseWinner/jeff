#include "history-processor.h"
#include <QDebug>

history_processor::history_processor(settings *_st, QObject *p) : QObject(p) {
  st = _st;
}

void history_processor::addVisibleMessage(AMessage *msg) {
  m_VisMsgHist.append(msg);
}

void history_processor::clearVisibleHistory() { m_VisMsgHist.clear(); }

void history_processor::exportVisibleHistory() {
  QString fn = QFileDialog::getSaveFileName(
      nullptr, "Save history", nullptr, "JSON File(*.json)");
  if (fn == QString()) return;
  auto *f = new QFile(fn);
  st->write(fromVisibleHistory(m_VisMsgHist), f);
}

void history_processor::deleteVisibleMessage(AMessage *msg) {
  m_VisMsgHist.removeOne(msg);
  msg->close();
}

QList<AMessage *> history_processor::fromTextHistory(QList<message> mh) {
  QList<AMessage *> vms;
  for (auto const &i : mh) vms.append(new AMessage(i));
  return vms;
}

QList<message> history_processor::fromVisibleHistory(QList<AMessage *> mh) {
  QList<message> th;
  for (auto i : mh)
    if (i != nullptr) th.append(i->returnDaemon());
  return th;
}
