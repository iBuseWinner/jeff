#ifndef CONTAINERS_H
#define CONTAINERS_H

#include <QDialog>
#include <QFileDialog>
#include <QString>
#include <QTreeWidgetItem>
#include "core/container.h"
#include "core/settings.h"
#include "core/sqlite.h"
#include "dialogues/createcontainer.h"
#include "widgets/a_button.h"
#include "widgets/a_containerslist.h"

class Containers : public QWidget {
 public:
  Containers(QWidget *p = nullptr);
  void add();
  void remove();

 private:
  Q_DISABLE_COPY(Containers)
  QMap<QTreeWidgetItem *, container> csm;
  CreateContainer *_cc = nullptr;
  AContainersList *cl = nullptr;
  QGridLayout *el = nullptr;
  AButton *ac = nullptr;
  AButton *cc = nullptr;
  AButton *rc = nullptr;
  AButton *ok = nullptr;
  bool ed = false;
  void connector();
  void lf();
  void app(const QList<container> &cProp);
  void cd(container _cProp);
  void sncl();
  void openCC();
  void closeCC();
};

#endif  // CONTAINERS_H
