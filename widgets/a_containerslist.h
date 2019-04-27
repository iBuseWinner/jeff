#ifndef A_CONTAINERSLIST_H
#define A_CONTAINERSLIST_H

#include <QTreeWidget>
#include <QTreeWidgetItem>

class AContainersList : public QTreeWidget {
  Q_OBJECT
 public:
  AContainersList(QWidget *p = nullptr);

 private:
  Q_DISABLE_COPY(AContainersList)
};

#endif  // A_CONTAINERSLIST_H
