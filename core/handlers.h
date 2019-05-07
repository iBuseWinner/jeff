#ifndef HANDLERS_H
#define HANDLERS_H

#include <QString>

class handlers {
 public:
  static QString purify(const QString& l);
  static QString removeCharacters(QString l);

 private:
  static QString ss() { return QString(".,:;!?-'\""); }
};

#endif  // HANDLERS_H
