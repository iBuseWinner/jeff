#ifndef CONTAINERS_H
#define CONTAINERS_H

#include "core/containersstruct.h"
#include "core/settingsstore.h"
#include "core/sqlite.h"
#include "dialogues/akiwake_dialog.h"
#include "dialogues/createcontainer.h"
#include "widgets/akiwake_containerslist.h"
#include "widgets/akiwake_pushbutton.h"
#include <QDialog>
#include <QFileDialog>
#include <QString>
#include <QTreeWidgetItem>

class Containers : public AkiwakeDialog {
public:
  explicit Containers(QWidget *parent = nullptr);
  void loadingFromFile();
  void appendCLTree(const QList<containerProperties>& Set);
  void addDB();
  void createDB();
  void removeDB();
  void disconnect();

private:
  Q_DISABLE_COPY(Containers)
  QMap<QTreeWidgetItem *, containerProperties> containers;
  void connector();
  QString openFile();
  AkiwakeContainersList *acl;
  AkiwakePushButton *addContainer;
  AkiwakePushButton *createContainer;
  AkiwakePushButton *disconnectContainer;
  AkiwakePushButton *removeContainer;
  AkiwakePushButton *ok;
  virtual void closeEvent(QCloseEvent *event);
};

#endif // CONTAINERS_H
