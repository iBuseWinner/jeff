#ifndef CONTAINERS_H
#define CONTAINERS_H

#include <QDialog>
#include <QFileDialog>
#include <QMenu>
#include <QString>
#include <QTranslator>
#include <QTreeWidgetItem>
#include "core/settings.h"
#include "dialogues/containers/createcontainer.h"
#include "widgets/a_button.h"
#include "widgets/a_containerslist.h"

/*!
 * Class: Containers
 * Manages NLP module containers.
 */
class Containers : public QWidget {
 public:
  // Functions:
  Containers(settings *_settings, QWidget *parent = nullptr);
  void add();
  void remove();

 private:
  Q_DISABLE_COPY(Containers)

  // Objects:
  settings *st = nullptr;
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
  void create(container _cProp);
  void sncl();
  void openCC();
  void closeCC();

  /*! Loads saved containers. */
  void load() { this->append(st->readContainerList()); }
};

#endif  // CONTAINERS_H
