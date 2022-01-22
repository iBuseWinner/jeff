#ifndef HISTORY_PROCESSOR_H
#define HISTORY_PROCESSOR_H

#include "core/basis.h"
#include "widgets/message.h"
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
  HProcessor(Basis *_basis, QObject *parent = nullptr) : QObject(parent) { basis = _basis; }

  /*!
   * @fn HProcessor::append
   * @brief Adds @a message to the story.
   * @param[in] message message to be added
   */
  void append(const MessageData &message) { _message_history.append(message); }

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
  void remove_one(MessageData message) { _message_history.removeOne(message); }

  // Functions described in `history-processor.cpp`:
  void save(const QString &filename);
  void load(const QString &filename);
  Messages recent(int amount);

signals:
  /*!
   * @brief Sends message history to Display.
   * @sa Display
   */
  Messages send_message_history(Messages message_history);

private:
  // Objects:
  Basis *basis = nullptr;
  Messages _message_history;
};

#endif
