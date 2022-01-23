#include "history-processor.h"

/*!
 * @fn HProcessor::load
 * @brief Loads @a _message_history from file.
 * @param[in] filename filename to read from
 */
void HProcessor::load(const QString &filename) {
  message_mutex.lock();
  message_history = basis->json->read_message_history(new QFile(filename)) + message_history;
  emit send_message_history(message_history);
  message_mutex.unlock();
}

/*!
  * @fn HProcessor::append
  * @brief Adds @a message to the story.
  * @param[in] message message to be added
  */
void HProcessor::append(const MessageData &message) {
  message_mutex.lock();
  message_history.append(message);
  message_mutex.unlock();
}

/*!
  * @fn HProcessor::clear
  * @brief Clears the history.
  */
void HProcessor::clear() {
  message_mutex.lock();
  message_history.clear();
  message_mutex.unlock();
}

/*!
  * @fn HProcessor::remove_one
  * @brief Removes message from history.
  * @param[in] message message to be removed
  */
void HProcessor::remove_one(MessageData message) {
  message_mutex.lock();
  message_history.removeOne(message);
  message_mutex.unlock();
}

/*!
 * @fn HProcessor::save
 * @brief Saves @a _message_history to file.
 * @param[in] filename filename to save there
 */
void HProcessor::save(const QString &filename) {
  message_mutex.lock();
  basis->json->write_message_history(message_history, new QFile(filename));
  message_mutex.unlock();
}

/*!
 * @fn HProcessor::recent
 * @brief Returns the latest messages.
 * @param[in] amount of recent messages
 */
Messages HProcessor::recent(int amount) {
  Messages messages;
  message_mutex.lock();
  if (amount > message_history.length())
    amount = message_history.length();
  if (amount > maximum_number_of_recent_messages_to_be_sent)
    amount = maximum_number_of_recent_messages_to_be_sent;
  for (int i = 0; i < amount; i++)
    messages.append(message_history[message_history.length() - i]);
  message_mutex.unlock();
  return messages;
}

/*!
 * @fn HProcessor::last_user_message
 * @brief Returns the last user message that did not include a template, or an empty string.
 */
QString HProcessor::last_user_message() {
  QString msg;
  message_mutex.lock();
  for (int i = message_history.length() - 1; i >= 0; i--) {
    if ((message_history[i].author == Author::User) and 
        (not message_history[i].content.startsWith("/"))) {
      msg = message_history[i].content;
      break;
    }
  }
  message_mutex.unlock();
  return msg;
}
