#ifndef MESSAGE_H
#define MESSAGE_H

#include <QString>

/*! Enum: eA [author]. */
enum eA { undefA, ASW, User };

/*! Enum: eC [content type]. */
enum eC { undefC, Text, Markdown, Picture, File, Warning, Error, Widget };

/*! Enum: eT [theme]. */
enum eT { undefT, Std, White, Dark, Red, Green, Blue, Yellow };

/*!
 * Struct: message.
 * Contains the message and its properties.
 */
struct message {
  /*! Content. */
  QString content = "";
  /*! Data and time of creation/change. */
  QString datetime = "";
  /*! Message author. */
  eA aType = eA::undefA;
  /*! Type of message content. */
  eC cType = eC::undefC;
  /*! Message theme. */
  eT tType = eT::Std;
};

#endif  // MESSAGE_H
