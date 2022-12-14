#ifndef MESSAGEDATA_H
#define MESSAGEDATA_H

#include <QDateTime>
#include <QJsonObject>
#include <QString>

/*!
 * @enum Author
 * @brief Enumeration of possible authors of messages.
 * @sa MessageData
 */
enum Author { undefA, Jeff, User };

/*!
 * @enum ContentType
 * @brief Enumeration of possible content types.
 * @sa MessageData
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
 * @brief Enumeration of possible message themes for Display.
 * @sa MessageData
 */
enum Theme { undefT, Std, White, Black, Red, Green, Blue, Yellow };

/*!
 * @class MessageData
 * @brief Contains the message and its properties.
 * @sa Author, ContentType, Theme
 */
class MessageData {
public:
  /*! Constructors. */
  MessageData() {}
  MessageData(const QJsonObject &json_object) {
    content = json_object["content"].toString();
    datetime = QDateTime::fromString(json_object["datetime"].toString(), Qt::ISODateWithMs);
    author = Author(json_object["author"].toInt());
    content_type = ContentType(json_object["content_type"].toInt());
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
  friend bool operator==(MessageData m1, MessageData m2) {
    return m1.author == m2.author and m1.content_type == m2.content_type and
           m1.content == m2.content and m1.datetime == m2.datetime;
  }
  /*! @brief Turns @a message into a JSON object.  */
  QJsonObject to_json() {
    return {{"content", content},
            {"datetime", datetime.toString(Qt::ISODateWithMs)},
            {"author", int(author)},
            {"content_type", int(content_type)},
            {"theme", int(theme)}};
  }
};

/*! @typedef Messages
 *  @brief Contains a part of message history.  */
typedef QList<MessageData> Messages;

#endif
