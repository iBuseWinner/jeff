#ifndef CONTAINERSSTRUCT_H
#define CONTAINERSSTRUCT_H

#include <QString>
struct containerProperties {
  QString path = ""; // path to database
  QString container = ""; // table name
  QString title = "";
  bool disabled = 0; // does not participate in the selection of reagents
  bool readOnly = 0; // not available for adding expressions from AS
  bool isPrivate = 0; // unavailable until purposeful entry
  bool isCatching = 0; // the search for reagents occurs exclusively in it
  bool isPriority = 0; // the input is first validated using this container
};

#endif // CONTAINERSSTRUCT_H
