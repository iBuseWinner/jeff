#ifndef CONTAINERS_H
#define CONTAINERS_H

#include <QDialog>
#include <QFileDialog>
#include <QMenu>
#include <QString>
#include <QTranslator>
#include <QTreeWidgetItem>
#include "dialogues/containers/createcontainer.h"
#include "widgets/a_button.h"
#include "widgets/a_containerslist.h"

/*!
 * Class: Containers.
 * Manages NLP module containers.
 */
class Containers : public QWidget {
 public:
  // Functions:
  explicit Containers(CoreMethods *_Meths, QWidget *parent = nullptr);
  void add();
  void remove();

 private:
  Q_DISABLE_COPY(Containers)

  // Objects:
  CoreMethods *Meths = nullptr;
  const QString objn = "containers";
  QMap<QTreeWidgetItem *, container> csm;
  CreateContainer *_cc = nullptr;
  AContainersList *cl = nullptr;
  QGridLayout *lt = nullptr;
  AButton *addBtn = nullptr;
  QAction *crtAct = nullptr;
  QAction *remAct = nullptr;
  AButton *snclBtn = nullptr;
  QAction *cancel = nullptr;
  bool edited = false;

  // Functions:
  void connector();
  void append(const QList<container> &cProps);
  void appendSingle(const container &cProp);
  void sncl();
  void openCC();
  void closeCC();
  void load();
};

#endif  // CONTAINERS_H
