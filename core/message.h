#ifndef MESSAGE_H
#define MESSAGE_H

#include <QString>

enum A { undefA, ASW, User };
enum C { undefC, Text, Markdown, Picture, File, Warning, Error, Widget };
enum T { undefT, Std, White, Dark, Red, Green, Blue, Yellow };

struct message {
  QString cn = QString();
  QString dt = QString();
  A aType = A::undefA;
  C cType = C::undefC;
  T tType = T::Std;
};

#endif  // MESSAGE_H
