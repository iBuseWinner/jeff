#ifndef A_SOURCELIST_H
#define A_SOURCELIST_H

#include <QTreeWidget>
#include <QTreeWidgetItem>

/*!
 * Class: ASourceList.
 * Container list widget.
 */
class ASourceList : public QTreeWidget {
  Q_OBJECT
public:
  // Functions:
  ASourceList(QWidget *parent = nullptr);

private:
  Q_DISABLE_COPY(ASourceList)
};

#endif // A_SOURCELIST_H
