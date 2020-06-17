#include "history-processor.h"
#include <QDebug>

/*!
 * Argument: QString {filename} [filename to save].
 * Saves {message_history} to file.
 */
void HProcessor::save(const QString &filename) {
  basis->writeMessageHistory(message_history, new QFile(filename));
}

/*!
 * Argument: QString {filename} [filename to load].
 * Loads {message_history} from file.
 */
void HProcessor::load(const QString &filename) {
  message_history =
      basis->readMessageHistory(new QFile(filename)) + message_history;
  emit sendMessageHistory(message_history);
}
