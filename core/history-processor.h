#ifndef HISTORY_PROCESSOR_H
#define HISTORY_PROCESSOR_H

#include <QFileDialog>
#include <QList>
#include <QStringList>
#include "core/settings.h"
#include "widgets/a_message.h"

class history_processor : public QObject {
  Q_OBJECT
 public:
  history_processor(settings *_st, QObject *p = nullptr);
  void addVisibleMessage(AMessage *msg);
  void clearVisibleHistory();
  void exportVisibleHistory();
  void deleteVisibleMessage(AMessage *msg);
  QList<AMessage *> loadHistory(const QString &fn);

 private:
  settings *st = nullptr;
  QList<AMessage *> m_VisMsgHist;
  QList<message> m_AllMsgHist;
  QList<AMessage *> fromTextHistory(QList<message> mh);
  QList<message> fromVisibleHistory(QList<AMessage *> mh);
};

#endif  // HISTORY_PROCESSOR_H
