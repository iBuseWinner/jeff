#ifndef HANDLERS_H
#define HANDLERS_H

#include <QString>

class handlers {
public:
  const char symbols[8] = {'.', ',', ':', ';', '!', '?', '-', '\''};
  QString purifyString(const QString& line);
  QString removeCharacters(QString line);
};

#endif // HANDLERS_H
