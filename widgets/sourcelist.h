#ifndef SOURCELIST_H
#define SOURCELIST_H

#include <QTreeWidget>

/*!
 * @class SourceList
 * @brief Source list widget.
 */
class SourceList : public QTreeWidget {
  Q_OBJECT
  Q_DISABLE_COPY(SourceList)
public:
  // Functions described in 'sourcelist.cpp':
  SourceList(QWidget *parent = nullptr);
};

#endif
