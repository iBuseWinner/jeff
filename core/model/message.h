#ifndef MESSAGE_H
#define MESSAGE_H

#include <QDateTime>
#include <QJsonObject>
#include <QString>

/*!
 * @enum Author
 * @brief Enumeration of possible authors of messages.
 * @sa Message
 */
enum Author { undefA, Jeff, User };

/*!
 * @enum ContentType
 * @brief Enumeration of possible content types.
 * @sa Message
 */
enum ContentType {
  undefC,
  Text,
  Markdown,
  Picture,
  File,
  Warning,
  Error,
  Widget
};

/*!
 * @enum Theme
 * @brief Enumeration of possible message themes for ADisplay.
 * @sa Message
 */
enum Theme { undefT, Std, White, Dark, Red, Green, Blue, Yellow };

/*!
 * @class Message
 * @brief Contains the message and its properties.
 * @sa Author, ContentType, Theme
 */
class Message {
public:
  /*! Constructors. */
  Message() {}
  Message(const QJsonObject &json_object) {
    content = json_object["content"].toString();
    datetime = QDateTime::fromString(json_object["datetime"].toString(),
        Qt::ISODateWithMs);
    author = Author(json_object["author"].toInt());
    content_type = ContentType(json_object["contentType"].toInt());
    theme = Theme(json_object["theme"].toInt());
  }
  /*! Content. */
  QString content = QString();
  /*! Data and time of creation/change. */
  QDateTime datetime;
  /*! Message author. */
  Author author = Author::undefA;
  /*! Type of message content. */
  ContentType content_type = ContentType::undefC;
  /*! Message theme. */
  Theme theme = Theme::Std;
  /*! Compares two messages. They are identical if the authors, content types,
   * content and posting times are the same. */
  friend bool operator==(Message m1, Message m2) {
    return m1.author == m2.author and m1.content_type == m2.content_type and
           m1.content == m2.content and m1.datetime == m2.datetime;
  }
  /*!
   * @fn to_json
   * @brief Turns @a message into a JSON object.
   * @param[in] message message data
   * @returns converted properties of @a message
   */
  QJsonObject to_json() {
    return {{"content", content},
            {"datetime", datetime.toString(Qt::ISODateWithMs)},
            {"author", int(author)},
            {"contentType", int(content_type)},
            {"theme", int(theme)}};
  }
};

/*!
 * @typedef Messages
 * @brief Contains a part of message history.
 */
typedef QList<Message> Messages;

#endif // MESSAGE_H
