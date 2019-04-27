#ifndef HANDLERS_H
#define HANDLERS_H

#include <QString>

class handlers {
 public:
  const char ss[9] = {'.', ',', ':', ';', '!', '?', '-', '\'', '"'};
  QString purifyString(const QString& l);
  QString removeCharacters(QString l);
};

#endif  // HANDLERS_H
