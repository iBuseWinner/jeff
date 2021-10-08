#ifndef SOURCELIST_H
#define SOURCELIST_H

#include <QTreeWidget>

/*!
 * @class SourceList
 * @brief Source list widget.
 */
class SourceList : public QTreeWidget {
  Q_OBJECT
public:
  // Functions described in 'sourcelist.cpp':
  explicit SourceList(QWidget *parent = nullptr);

private:
  Q_DISABLE_COPY(SourceList)
};

#endif
