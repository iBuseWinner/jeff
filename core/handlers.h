#ifndef HANDLERS_H
#define HANDLERS_H

#include <QString>

/*!
 * Class: handlers.
 * Contains methods for working with strings.
 */
class handlers {
 public:
  // Functions:
  static QString purify(const QString& str);
  static QString removeCharacters(QString str);

 private:
  // Functions:
  /*! Returns punctuation. */
  static QString sbs() { return QString(".,:;!?-'\""); }
};

#endif  // HANDLERS_H
