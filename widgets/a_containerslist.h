#ifndef A_CONTAINERSLIST_H
#define A_CONTAINERSLIST_H

#include <QFontDatabase>
#include <QTreeWidget>
#include <QTreeWidgetItem>

class AContainersList : public QTreeWidget {
  Q_OBJECT
public:
  AContainersList(QWidget *parent = nullptr);

private:
  Q_DISABLE_COPY(AContainersList)
};

#endif // A_CONTAINERSLIST_H
