#ifndef HISTORY_PROCESSOR_H
#define HISTORY_PROCESSOR_H

#include "core/basis.h"
#include "widgets/a_message.h"
#include <QFileDialog>
#include <QList>
#include <QStringList>

/*!
 * @class HProcessor
 * @brief Stores, exports and loads message history.
 * @sa Message, Basis
 */
class HProcessor : public QObject {
  Q_OBJECT
public:
  // Functions:
  /*! The constructor. */
  HProcessor(Basis *basis, QObject *parent = nullptr)
      : QObject(parent), _basis(basis) {}
  /*! Adds a message to the story. */
  void append(const Message &message) { _message_history.append(message); }
  /*! Clears the history. */
  void clear() { _message_history.clear(); }
  /*! Removes message from history. */
  void remove_one(Message message) { _message_history.removeOne(message); }

  // Functions described in `history-processor.cpp`:
  void save(const QString &filename);
  void load(const QString &filename);

signals:
  QList<Message> send_message_history(QList<Message> message_history);

private:
  // Objects:
  Basis *_basis = nullptr;
  QList<Message> _message_history;
};

#endif // HISTORY_PROCESSOR_H
