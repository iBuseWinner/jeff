#ifndef CONTAINERSSTRUCT_H
#define CONTAINERSSTRUCT_H

#include <QString>

struct container {
  QString p = QString();  // path to database
  QString c = QString();  // table path
  QString t = QString();  // table name
  bool d = 0;      // does not participate in the selection of reagents
  bool ro = 0;     // not available for adding expressions from ASW
  bool pv = 0;     // unavailable until purposeful entry
  bool ch = 0;     // the search for reagents occurs exclusively in it
  bool pr = 0;     // the input is first validated using this container
};

#endif  // CONTAINERSSTRUCT_H
