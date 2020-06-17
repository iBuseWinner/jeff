#ifndef HISTORY_PROCESSOR_H
#define HISTORY_PROCESSOR_H

#include "core/basis.h"
#include "widgets/a_message.h"
#include <QFileDialog>
#include <QList>
#include <QStringList>

/*!
 * Class: HProcessor.
 * Stores, exports and loads message history.
 */
class HProcessor : public QObject {
  Q_OBJECT
public:
  // Functions:
  HProcessor(Basis *_basis, QObject *parent = nullptr)
      : QObject(parent), basis(_basis) {}
  void save(const QString &filename);
  void load(const QString &filename);

  /*! Adds a message to the story. */
  void append(const Message &_message) { message_history.append(_message); }
  /*! Clears the history. */
  void clear() { message_history.clear(); }
  /*! Removes message from history. */
  void removeOne(Message _message) { message_history.removeOne(_message); }

signals:
  QList<Message> sendMessageHistory(QList<Message> _message_history);

private:
  // Objects:
  Basis *basis = nullptr;
  QList<Message> message_history;
};

#endif // HISTORY_PROCESSOR_H
