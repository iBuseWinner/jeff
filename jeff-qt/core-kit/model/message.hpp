#ifndef MESSAGE_H
#define MESSAGE_H

#include <QDateTime>
#include <QJsonObject>
#include <QString>

/*!
 * @enum Author
 * @brief Enumeration of possible authors of messages.
 * @sa MessageMeta
 */
enum Author { undefA, Jeff, User };

/*!
 * @enum ContentType
 * @brief Enumeration of possible content types.
 * @sa MessageMeta
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
 * @sa MessageMeta
 */
enum Theme { undefT, Std, White, Black, Red, Green, Blue, Yellow };

/*!
 * @class MessageMeta
 * @brief Contains the message and its properties.
 * @sa Author, ContentType, Theme
 */
class MessageMeta {
public:
  /*! Constructors. */
  MessageMeta() {}
  MessageMeta(const QJsonObject &json_object) {
    content = json_object["content"].toString();
    datetime = QDateTime::fromString(json_object["datetime"].toString(), Qt::ISODateWithMs);
    author = Author(json_object["author"].toInt());
    content_type = ContentType(json_object["content_type"].toInt());
    theme = Theme(json_object["theme"].toInt());
  }
  /*! Content. */
  QString content = "";
  /*! Data and time of creation/change. */
  QDateTime datetime = QDateTime::currentDateTime();
  /*! Message author. */
  Author author = Author::undefA;
  /*! Type of message content. */
  ContentType content_type = ContentType::undefC;
  /*! Message theme. */
  Theme theme = Theme::Std;
  /*! Compares two messages. They are identical if the authors, content types,
   * content and posting times are the same. */
  friend bool operator==(MessageMeta m1, MessageMeta m2) {
    return m1.author == m2.author and m1.content_type == m2.content_type and
           m1.content == m2.content and m1.datetime == m2.datetime;
  }
  /*! @brief Turns @a message into a JSON object.  */
  QJsonObject to_json() const {
    return {{"content", content},
            {"datetime", datetime.toString(Qt::ISODateWithMs)},
            {"author", int(author)},
            {"content_type", int(content_type)},
            {"theme", int(theme)}};
  }
};

/*! @typedef MessagesMeta
 *  @brief Contains a part of message history.  */
typedef QList<MessageMeta> MessagesMeta;

#endif
