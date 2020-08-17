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
  /*!
   * @fn HProcessor::HProcessor
   * @brief The constructor.
   * @param[in,out] basis reference to the Basis instance
   * @param[in,out] parent QObject parent
   */
  HProcessor(Basis *basis, QObject *parent = nullptr)
      : QObject(parent), _basis(basis) {}

  /*!
   * @fn HProcessor::append
   * @brief Adds @a message to the story.
   * @param[in] message message to be added
   */
  void append(const Message &message) { _message_history.append(message); }

  /*!
   * @fn HProcessor::clear
   * @brief Clears the history.
   */
  void clear() { _message_history.clear(); }

  /*!
   * @fn HProcessor::remove_one
   * @brief Removes message from history.
   * @param[in] message message to be removed
   */
  void remove_one(Message message) { _message_history.removeOne(message); }

  // Functions described in `history-processor.cpp`:
  void save(const QString &filename);
  void load(const QString &filename);

signals:
  /*!
   * @brief Sends message history to ADisplay.
   * @sa ADisplay
   */
  QList<Message> send_message_history(QList<Message> message_history);

private:
  // Objects:
  Basis *_basis = nullptr;
  QList<Message> _message_history;
};

#endif // HISTORY_PROCESSOR_H
