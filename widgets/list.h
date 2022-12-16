#ifndef LIST_H
#define LIST_H

#include "widgets/styling.h"
#include <QMutex>
#include <QScrollBar>
#include <QTreeWidget>

/*! @class List
 *  @brief List widget.  */
class List : public QTreeWidget {
  Q_OBJECT
  Q_DISABLE_COPY(List)
public:
  // Functions described in `list.cpp`:
  List(QWidget *parent = nullptr);
  void addTopLevelItem(QTreeWidgetItem *item);
  void clear();
  
private:
  // Objects:
  QList<QTreeWidgetItem *> top_level_items;
  int items_counter = 0;
  short normal_portion;
  QMutex scroll_update;
  
  // Constants:
  const short max_items_amount = 100;
  
  // Functions described in `list.cpp`:
  void scroll(int min, int max);
  void show_items(int value);
};

#endif
