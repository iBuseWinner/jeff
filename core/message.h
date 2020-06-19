#ifndef MESSAGE_H
#define MESSAGE_H

#include <QDateTime>
#include <QString>

/*!
 * @enum Author
 * @brief Enumeration of possible authors of messages.
 * @sa Message
 */
enum Author { undefA, ASW, User };

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
 * @struct Message
 * @brief Contains the message and its properties.
 * @sa Author, ContentType, Theme
 */
struct Message {
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

  friend bool operator==(Message m1, Message m2) {
    return m1.author == m2.author and m1.content_type == m2.content_type and
           m1.content == m2.content and m1.datetime == m2.datetime;
  }
};

#endif // MESSAGE_H
