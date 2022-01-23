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
  /*!
   * @fn HProcessor::HProcessor
   * @brief The constructor.
   * @param[in,out] basis reference to the Basis instance
   * @param[in,out] parent QObject parent
   */
  HProcessor(Basis *_basis, QObject *parent = nullptr) : QObject(parent) { basis = _basis; }

  // Functions described in `history-processor.cpp`:
  void load(const QString &filename);
  void append(const MessageData &message);
  void clear();
  void remove_one(MessageData message);
  void save(const QString &filename);
  Messages recent(int amount);
  QString last_user_message();

signals:
  /*!
   * @brief Sends message history to Display.
   * @sa Display
   */
  Messages send_message_history(Messages message_history);

private:
  // Objects:
  Basis *basis = nullptr;
  QMutex message_mutex;
  Messages message_history;
  
  // Constants:
  const int maximum_number_of_recent_messages_to_be_sent = 20;
};

#endif
