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
};

#endif // MESSAGE_H
