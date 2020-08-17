#include "history-processor.h"

/*!
 * @fn HProcessor::save
 * @brief Saves @a _message_history to file.
 * @param[in] filename filename to save there
 */
void HProcessor::save(const QString &filename) {
  _basis->write_message_history(_message_history, new QFile(filename));
}

/*!
 * @fn HProcessor::load
 * @brief Loads @a _message_history from file.
 * @param[in] filename filename to read from
 */
void HProcessor::load(const QString &filename) {
  _message_history =
      _basis->read_message_history(new QFile(filename)) + _message_history;
  emit send_message_history(_message_history);
}
