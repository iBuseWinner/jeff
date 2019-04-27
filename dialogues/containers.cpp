#include "containers.h"

Containers::Containers(QWidget *p) : QWidget(p) {
  setAttribute(Qt::WA_DeleteOnClose);
  // Creates main objects...
  el = new QGridLayout();
  el->setSpacing(0);
  el->setMargin(0);
  ac = new APushButton("Add container", this);
  cc = new APushButton("Create", this);
  rc = new APushButton("Remove", this);
  ok = new APushButton("Save and close", this);
  ac->setIcon(QIcon(":/arts/icons/16/insert-link.svg"));
  cc->setIcon(QIcon(":/arts/icons/16/document-new.svg"));
  rc->setIcon(QIcon(":/arts/icons/16/remove-link.svg"));
  ok->setIcon(QIcon(":/arts/icons/16/dialog-ok-apply.svg"));
  cl = new AContainersList(this);
  el->addWidget(cl, 0, 0, 1, 0);
  el->addWidget(ac, 1, 0);
  el->addWidget(cc, 1, 1);
  el->addWidget(rc, 1, 2);
  el->addWidget(ok, 1, 3);
  setLayout(el);
  connector();
  lf();
}

void Containers::connector() {
  // Connects signals with slots.
  connect(ac, &APushButton::clicked, this, &Containers::add);
  connect(cc, &APushButton::clicked, this, &Containers::openCC);
  connect(rc, &APushButton::clicked, this, &Containers::remove);
  connect(ok, &APushButton::clicked, this, &Containers::sncl);
}

void Containers::sncl() {
  if (ed) {
    QList<containerProperties> cProps;
    for (int i = 0; i < cl->topLevelItemCount(); i++)
      for (int j = 0; j < cl->invisibleRootItem()->child(i)->childCount(); j++)
        cProps.append(csm.value(cl->invisibleRootItem()->child(i)->child(j)));
    SettingsStore st;
    st.write(cProps);
  }
  parentWidget()->parentWidget()->close();
}

void Containers::lf() {
  SettingsStore st;
  QList<containerProperties> cProps = st.read();
  this->app(cProps);
}

void Containers::cd(containerProperties _cProp) {
  sqlite sq;
  sq.create(_cProp);
  QList<containerProperties> cProps;
  cProps.append(_cProp);
  ed = true;
  app(cProps);
}

void Containers::add() {
  QString p = QFileDialog::getOpenFileName(this, "Select database", nullptr,
                                           "ASW database(*.asw.db)");
  if (p.isEmpty()) return;
  sqlite sq;
  QList<containerProperties> cProps = sq.containers(p);
  ed = true;
  app(cProps);
}

void Containers::app(const QList<containerProperties> &cProp) {
  for (const auto &i : cProp) {
    QTreeWidgetItem *e = nullptr;
    bool in = false;
    for (int j = 0; j < cl->topLevelItemCount(); j++)
      if (cl->invisibleRootItem()->child(j)->text(0) == i.p) {
        in = true;
        e = cl->invisibleRootItem()->takeChild(j);
        break;
      }
    if (!in) {
      e = new QTreeWidgetItem();
      e->setText(0, i.p);
    }
    cl->addTopLevelItem(e);
    bool notContains = true;
    for (int j = 0; j < e->childCount(); j++)
      if (e->child(j)->text(0) == i.t) {
        notContains = false;
        break;
      }
    if (notContains) {
      auto *ce = new QTreeWidgetItem(e);
      ce->setText(0, i.t);
      csm.insert(ce, i);
    }
  }
}

void Containers::openCC() {
  disconnect(cc, &APushButton::clicked, this, &Containers::openCC);
  _cc = new CreateContainer(this);
  el->addWidget(_cc, 2, 0, 1, 0);
  connect(_cc, &CreateContainer::cont, this, &Containers::cd);
  connect(_cc, &CreateContainer::c, this, &Containers::closeCC);
}

void Containers::closeCC() {
  disconnect(_cc, &CreateContainer::cont, this, &Containers::cd);
  disconnect(_cc, &CreateContainer::c, this, &Containers::closeCC);
  el->removeWidget(_cc);
  delete _cc;
  connect(cc, &APushButton::clicked, this, &Containers::openCC);
}

void Containers::remove() {
  if (cl->selectedItems().length() < 1) return;
  ed = true;
  for (int i = 0; i < cl->selectedItems().length(); i++) {
    QTreeWidgetItem *b;
    if (cl->selectedItems().at(i)->parent() == nullptr)
      b = cl->invisibleRootItem();
    else
      b = cl->selectedItems().at(i)->parent();
    b->removeChild(cl->selectedItems().at(i));
    if ((b->childCount() == 0) && (b != cl->invisibleRootItem()))
      cl->invisibleRootItem()->removeChild(b);
  }
}
