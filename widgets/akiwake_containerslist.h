#ifndef AKIWAKE_CONTAINERSLIST_H
#define AKIWAKE_CONTAINERSLIST_H

#include <QFontDatabase>
#include <QTreeWidget>
#include <QTreeWidgetItem>

class AkiwakeContainersList : public QTreeWidget {
  Q_OBJECT
public:
  AkiwakeContainersList(QWidget *parent = nullptr);

private:
  Q_DISABLE_COPY(AkiwakeContainersList)
};

#endif // AKIWAKE_CONTAINERSLIST_H
