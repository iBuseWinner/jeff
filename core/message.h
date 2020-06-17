#ifndef MESSAGE_H
#define MESSAGE_H

#include <QDateTime>
#include <QString>

/*! Enum: Author. */
enum Author { undefA, ASW, User };

/*! Enum: Content type. */
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

/*! Enum: Theme. */
enum Theme { undefT, Std, White, Dark, Red, Green, Blue, Yellow };

/*!
 * Struct: Message.
 * Contains the message and its properties.
 */
struct Message {
  /*! Content. */
  QString content = "";
  /*! Data and time of creation/change. */
  QDateTime datetime;
  /*! Message author. */
  Author aType = Author::undefA;
  /*! Type of message content. */
  ContentType cType = ContentType::undefC;
  /*! Message theme. */
  Theme tType = Theme::Std;

  friend bool operator==(Message m1, Message m2) {
    return m1.aType == m2.aType and m1.cType == m2.cType and
           m1.content == m2.content and m1.datetime == m2.datetime;
  }
};

#endif // MESSAGE_H
