#ifndef CONTAINERS_H
#define CONTAINERS_H

#include <QDialog>
#include <QFileDialog>
#include <QString>
#include <QTreeWidgetItem>
#include "core/containersstruct.h"
#include "core/settingsstore.h"
#include "core/sqlite.h"
#include "dialogues/createcontainer.h"
#include "widgets/a_containerslist.h"
#include "widgets/a_pushbutton.h"

class Containers : public QWidget {
 public:
  Containers(QWidget *p = nullptr);
  void add();
  void remove();

 private:
  Q_DISABLE_COPY(Containers)
  bool ed = false;
  void lf();
  void connector();
  QString openF();
  void app(const QList<containerProperties> &cProp);
  void cd(containerProperties _cProp);
  void sncl();
  void openCC();
  void closeCC();
  QGridLayout *el = nullptr;
  QMap<QTreeWidgetItem *, containerProperties> csm;
  AContainersList *cl = nullptr;
  APushButton *ac = nullptr;
  APushButton *cc = nullptr;
  APushButton *rc = nullptr;
  APushButton *ok = nullptr;
  CreateContainer *_cc = nullptr;
};

#endif  // CONTAINERS_H
