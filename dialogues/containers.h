#ifndef CONTAINERS_H
#define CONTAINERS_H

#include "core/containersstruct.h"
#include "core/settingsstore.h"
#include "core/sqlite.h"
#include "dialogues/createcontainer.h"
#include "widgets/a_containerslist.h"
#include "widgets/a_pushbutton.h"
#include <QDialog>
#include <QFileDialog>
#include <QString>
#include <QTreeWidgetItem>

class Containers : public QWidget {
public:
  Containers(QWidget *parent = nullptr);
  void loadingFromFile();
  void appendCLTree(const QList<containerProperties> &Set);
  void addDB();
  // void createDB();
  // void disconnect();
  void removeDB();

private:
  Q_DISABLE_COPY(Containers)
  void connector();
  void saveAndClose();
  QString openFile();
  QMap<QTreeWidgetItem *, containerProperties> containers;
  AContainersList *acl = nullptr;
  APushButton *addContainer = nullptr;
  // AkiwakePushButton *createContainer;
  // AkiwakePushButton *disconnectContainer;
  APushButton *removeContainer = nullptr;
  APushButton *ok = nullptr;
};

#endif // CONTAINERS_H
