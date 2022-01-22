#include "history-processor.h"

/*!
 * @fn HProcessor::save
 * @brief Saves @a _message_history to file.
 * @param[in] filename filename to save there
 */
void HProcessor::save(const QString &filename) {
  basis->json->write_message_history(_message_history, new QFile(filename));
}

/*!
 * @fn HProcessor::load
 * @brief Loads @a _message_history from file.
 * @param[in] filename filename to read from
 */
void HProcessor::load(const QString &filename) {
  _message_history =
      basis->json->read_message_history(new QFile(filename)) + _message_history;
  emit send_message_history(_message_history);
}

Messages HProcessor::recent(int amount) {
  Messages messages;
  Messages mh_clone = Messages(_message_history);
  if (amount > mh_clone.length())
    amount = mh_clone.length();
  for (int i = 0; i < amount; i++)
    messages.append(mh_clone[mh_clone.length() - i]);
  return messages;
}
