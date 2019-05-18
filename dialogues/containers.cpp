#include "containers.h"

/*
 * All short named objects and their explanations:
 * {st} <- settings
 * {objn} <- object name
 * {lt} <- layout
 * {addBtn} <- add container
 * {crtBtn} <- create container
 * {remBtn} <- remove container
 * {snclBtn} <- save and close
 * {cl} <- container list
 * {csm} <- containers map
 * {p} <- path
 * {cProps} <- containers properties
 * {cProp} <- container properties
 * {_cc} <- CreateContainer
 * {tli2} <- top-level item index
 */

/*!
 * Arguments: settings {_settings} [reference to Settings instance],
 *            QWidget {*parent}.
 * Constructs and prepares Container Manager.
 */
Containers::Containers(settings *_settings, QWidget *parent) : QWidget(parent) {
  st = _settings;
  setAttribute(Qt::WA_DeleteOnClose);
  setObjectName(objn);
  lt = new QGridLayout();
  lt->setSpacing(0);
  lt->setMargin(0);
  addBtn = new AButton(tr("Add container"), this);
  crtBtn = new AButton(tr("Create"), this);
  remBtn = new AButton(tr("Remove"), this);
  snclBtn = new AButton(tr("Save and close"), this);
  addBtn->setIcon(QIcon(":/arts/icons/16/insert-link.svg"));
  crtBtn->setIcon(QIcon(":/arts/icons/16/document-new.svg"));
  remBtn->setIcon(QIcon(":/arts/icons/16/remove-link.svg"));
  snclBtn->setIcon(QIcon(":/arts/icons/16/dialog-ok-apply.svg"));
  cl = new AContainersList(this);
  lt->addWidget(cl, 0, 0, 1, 0);
  lt->addWidget(addBtn, 1, 0);
  lt->addWidget(crtBtn, 1, 1);
  lt->addWidget(remBtn, 1, 2);
  lt->addWidget(snclBtn, 1, 3);
  setLayout(lt);
  connector();
  load();
}

/*! Adds a container to the widget, loads its data. */
void Containers::add() {
  QString p =
      QFileDialog::getOpenFileName(nullptr, tr("Select database"), nullptr,
                                   tr("ASW database") + "(*.asw.db)");
  if (p.isEmpty()) return;
  edited = true;
  append(st->SQL->containers(p));
}

/*! Removes selected containers. */
void Containers::remove() {
  if (cl->selectedItems().length() < 1) return;
  edited = true;
  for (int i = 0; i < cl->selectedItems().length(); i++) {
    QTreeWidgetItem *parent;
    if (cl->selectedItems().at(i)->parent() == nullptr)
      parent = cl->invisibleRootItem();
    else
      parent = cl->selectedItems().at(i)->parent();
    parent->removeChild(cl->selectedItems().at(i));
    if ((parent->childCount() == 0) && (parent != cl->invisibleRootItem()))
      cl->invisibleRootItem()->removeChild(parent);
  }
}

/*! Establishes communications for user interaction through the dialog box. */
void Containers::connector() {
  connect(addBtn, &AButton::clicked, this, &Containers::add);
  connect(crtBtn, &AButton::clicked, this, &Containers::openCC);
  connect(remBtn, &AButton::clicked, this, &Containers::remove);
  connect(snclBtn, &AButton::clicked, this, &Containers::sncl);
}

/*!
 * Argument: QList of containers {cProps}.
 * Adds containers to the selection.
 */
void Containers::append(const QList<container> &cProps) {
  for (const auto &cProp : cProps) {
    QTreeWidgetItem *parent = nullptr;
    bool isInside = false;
    for (int tli2 = 0; tli2 < cl->topLevelItemCount(); tli2++)
      if (cl->invisibleRootItem()->child(tli2)->text(0) == cProp.path) {
        isInside = true;
        parent = cl->invisibleRootItem()->takeChild(tli2);
        break;
      }
    if (!isInside) parent = new QTreeWidgetItem(QStringList(cProp.path));
    cl->addTopLevelItem(parent);
    bool notContains = true;
    for (int childIndex = 0; childIndex < parent->childCount(); childIndex++)
      if (parent->child(childIndex)->text(0) == cProp.tableTitle) {
        notContains = false;
        break;
      }
    if (notContains) {
      auto *cell = new QTreeWidgetItem(parent, QStringList(cProp.tableTitle));
      csm.insert(cell, cProp);
    }
  }
}

/*! Creates a container and adds it to the selection. */
void Containers::create(container _cProp) {
  st->SQL->create(_cProp);
  _cProp.tableName = st->SQL->getUuid();
  QList<container> cProps;
  cProps.append(_cProp);
  edited = true;
  append(cProps);
}

/*! Saves the selection and closes the dialog. */
void Containers::sncl() {
  if (edited) {
    QList<container> cProps;
    for (int tli2 = 0; tli2 < cl->topLevelItemCount(); tli2++)
      for (int childIndex = 0;
           childIndex < cl->invisibleRootItem()->child(tli2)->childCount();
           childIndex++)
        cProps.append(
            csm.value(cl->invisibleRootItem()->child(tli2)->child(childIndex)));
    st->writeContainerList(cProps);
  }
  close();
}

/*! Opens the container creation dialog. */
void Containers::openCC() {
  disconnect(crtBtn, &AButton::clicked, this, &Containers::openCC);
  _cc = new CreateContainer(this);
  lt->addWidget(_cc, 2, 0, 1, 0);
  connect(_cc, &CreateContainer::completed, this, &Containers::create);
  connect(_cc, &CreateContainer::cancelled, this, &Containers::closeCC);
}

/*! Closes the container creation dialog. */
void Containers::closeCC() {
  disconnect(_cc, &CreateContainer::completed, this, &Containers::create);
  disconnect(_cc, &CreateContainer::cancelled, this, &Containers::closeCC);
  lt->removeWidget(_cc);
  connect(crtBtn, &AButton::clicked, this, &Containers::openCC);
}
