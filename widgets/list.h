#ifndef LIST_H
#define LIST_H

#include "widgets/styling.h"
#include <QTreeWidget>

/*!
 * @class List
 * @brief List widget.
 */
class List : public QTreeWidget {
  Q_OBJECT
  Q_DISABLE_COPY(List)
public:
  // Functions described in `list.cpp`:
  List(QWidget *parent = nullptr);
};

#endif
