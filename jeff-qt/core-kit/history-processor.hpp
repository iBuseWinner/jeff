#ifndef HISTORY_PROCESSOR_H
#define HISTORY_PROCESSOR_H

#include "core-kit/basis.hpp"
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
  HProcessor(Basis *_basis, QObject *parent = nullptr) : QObject(parent), basis(_basis) {}

  // Functions described in `history-processor.cpp`:
  void load(const QString &filename = QString());
  void append(MessageMeta *message);
  void clear();
  void remove_one(MessageMeta *message);
  void save(const QString &filename = QString());
  MessagesMeta recent(int amount);
  QString last_user_message(int i = 0);
  MessagesMeta *messages();
  int length();

signals:
  /*! @brief Sends message history to Display. */
  MessagesMeta history_loaded(MessagesMeta message_history);

private:
  // Objects:
  Basis *basis = nullptr;
  MessagesMeta message_history = MessagesMeta();

  // Constants:
  const int maximum_number_of_recent_messages_to_be_sent = 20;
};

#endif
