#ifndef A_CONTAINERSLIST_H
#define A_CONTAINERSLIST_H

#include <QTreeWidget>
#include <QTreeWidgetItem>

/*!
 * Class: AContainersList
 * Container list widget.
 */
class AContainersList : public QTreeWidget {
  Q_OBJECT
 public:
  // Functions:
  AContainersList(QWidget *parent = nullptr);

 private:
  Q_DISABLE_COPY(AContainersList)
};

#endif  // A_CONTAINERSLIST_H
