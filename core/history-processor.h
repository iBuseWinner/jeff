#ifndef HISTORY_PROCESSOR_H
#define HISTORY_PROCESSOR_H

#include "core/basis.h"
#include "widgets/message.h"
#include <QFileDialog>
#include <QList>
#include <QMutex>
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
  /*! @brief The constructor. */
  HProcessor(Basis *_basis, QObject *parent = nullptr) : QObject(parent) { basis = _basis; }

  // Functions described in `history-processor.cpp`:
  void load(const QString &filename = QString());
  void append(const MessageData &message);
  void clear();
  void remove_one(MessageData message);
  void save(const QString &filename = QString());
  Messages recent(int amount);
  QString last_user_message();

signals:
  /*! @brief Sends message history to Display. */
  Messages history_loaded(Messages message_history);

private:
  // Objects:
  Basis *basis = nullptr;
  QMutex message_mutex;
  Messages message_history;
  
  // Constants:
  const int maximum_number_of_recent_messages_to_be_sent = 20;
};

#endif
