#ifndef HISTORY_PROCESSOR_H
#define HISTORY_PROCESSOR_H

#include <QFileDialog>
#include <QList>
#include <QStringList>
#include "core/basis.h"
#include "widgets/a_message.h"

/*!
 * Class: historyProcessor.
 * Stores, exports and loads message history.
 */
class HProcessor : public QObject {
  Q_OBJECT
 public:
  // Functions:
  HProcessor(Basis *_basis, QObject *parent = nullptr);
  void save(const QString &filename);
  void load(const QString &filename);
  void removeOne(Message _message);

  /*! Adds a message to the story. */
  void append(const Message &_message) { mh.append(_message); }
  /*! Clears the history. */
  void clear() { mh.clear(); }

 signals:
  QList<Message> sendMessageHistory(QList<Message> _mh);

 private:
  // Objects:
  Basis *basis = nullptr;
  QList<Message> mh;
};

#endif  // HISTORY_PROCESSOR_H
