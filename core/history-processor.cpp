#include "history-processor.h"

/*! @brief Loads @a _message_history from file. */
void HProcessor::load(const QString &filename) {
  message_mutex.lock();
  if (filename.isEmpty())
    message_history = basis->json->read_message_history();
  else
    message_history = basis->json->read_message_history(new QFile(filename)) + message_history;
  emit history_loaded(message_history);
  message_mutex.unlock();
}

/*! @brief Adds @a message to the story. */
void HProcessor::append(const MessageData &message) {
  message_mutex.lock();
  message_history.append(message);
  message_mutex.unlock();
}

/*! @brief Clears the history. */
void HProcessor::clear() {
  message_mutex.lock();
  message_history.clear();
  message_mutex.unlock();
}

/*! @brief Removes message from history. */
void HProcessor::remove_one(MessageData message) {
  message_mutex.lock();
  message_history.removeOne(message);
  message_mutex.unlock();
}

/*! @brief Saves @a _message_history to file. */
void HProcessor::save(const QString &filename) {
  message_mutex.lock();
  Messages _message_history;
  for (auto msg : message_history)
    if (msg.content_type != ContentType::Widget) _message_history.append(msg);
  message_mutex.unlock();
  if (filename.isEmpty())
    basis->json->write_message_history(_message_history);
  else
    basis->json->write_message_history(_message_history, new QFile(filename));
}

/*! @brief Returns the latest messages. */
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

/*! @brief Returns the last user message that did not include a template, or an empty string. */
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
