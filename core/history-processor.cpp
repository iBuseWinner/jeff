#include "history-processor.h"
#include <iostream>

/*! @brief Loads @a _message_history from file. */
void HProcessor::load(const QString &filename) {
  if (filename.isEmpty())
    message_history = basis->json->read_message_history();
  else
    message_history = basis->json->read_message_history(new QFile(filename, this)) + message_history;
  emit history_loaded(message_history);
}

/*! @brief Adds @a message to the story. */
void HProcessor::append(const MessageData &message) { message_history.append(message); }
/*! @brief Clears the history. */
void HProcessor::clear() { message_history.clear(); }
/*! @brief Removes message from history. */
void HProcessor::remove_one(MessageData message) { message_history.removeOne(message); }

/*! @brief Saves @a _message_history to file. */
void HProcessor::save(const QString &filename) {
  Messages _message_history;
  for (auto msg : message_history)
    if (msg.content_type != ContentType::Widget) _message_history.append(msg);
  if (filename.isEmpty())
    basis->json->write_message_history(_message_history);
  else
    basis->json->write_message_history(_message_history, new QFile(filename));
}

/*! @brief Returns the latest messages. */
Messages HProcessor::recent(int amount) {
  Messages messages;
  if (amount > message_history.length())
    amount = message_history.length();
  if (amount > maximum_number_of_recent_messages_to_be_sent)
    amount = maximum_number_of_recent_messages_to_be_sent;
  for (int i = 0; i < amount; i++) {
    messages.append(message_history[message_history.length() - i - 1]);
  }
  return messages;
}

/*! @brief Returns the last user message that did not include a template, or an empty string. */
QString HProcessor::last_user_message(int i) {
  QString msg = "";
  for (int j = message_history.length() - 1; j >= 0; j--) {
    if ((message_history[j].author == Author::User) and 
        (not message_history[j].content.startsWith("/"))) {
      if (i-- > 0) continue; 
      msg = message_history[j].content;
      break;
    }
  }
  return msg;
}

/*! @brief Blocks the addition of new messages and passes the history to the calling method. */
Messages *HProcessor::messages() { return &message_history; }
/*! @brief Returns the length of the history. */
int HProcessor::length() { return message_history.length(); }
