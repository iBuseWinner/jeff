#include "sources.h"

/*
 * All short named objects and their explanations:
 * {basis} <- core methods
 * {objn} <- object name
 * {lt} <- layout
 * {addBtn} <- add container
 * {crtAct} <- create container
 * {remAct} <- remove container
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
 * Arguments: Basis {_basis} [reference to Basis instance],
 *            QWidget {*parent}.
 * Constructs and prepares Source Manager.
 *
 * Layout scheme:
 * <----------------------->
 * [     Sources  list     ]
 * [Add button][S&C  button]
 * {|> Create }
 * {|> Remove }
 * <----------------------->
 */
SourcesDialog::SourcesDialog(Basis *_basis, QWidget *parent) : QWidget(parent) {
  basis = _basis;
  setAttribute(Qt::WA_DeleteOnClose);
  setObjectName(objn);
  lt = new QGridLayout();
  lt->setSpacing(0);
  lt->setMargin(0);
  addBtn = new AButton(QTranslator::tr("Add container"), this);
  crtAct = new QAction(QTranslator::tr("Create"), this);
  remAct = new QAction(QTranslator::tr("Remove"), this);
  snclBtn = new AButton(QTranslator::tr("Save and close"), this);
  addBtn->setIcon(QIcon(":/arts/icons/16/insert-link.svg"));
  crtAct->setIcon(QIcon(":/arts/icons/16/document-new.svg"));
  remAct->setIcon(QIcon(":/arts/icons/16/remove-link.svg"));
  snclBtn->setIcon(QIcon(":/arts/icons/16/dialog-ok-apply.svg"));
  addBtn->setPopupMode(QToolButton::MenuButtonPopup);
  addBtn->addAction(crtAct);
  addBtn->addAction(remAct);
  cl = new AContainersList(this);
  lt->addWidget(cl, 0, 0, 1, 0);
  lt->addWidget(addBtn, 1, 0);
  lt->addWidget(snclBtn, 1, 1);
  setLayout(lt);
  connector();
  load();
}

/*! Loads saved containers. */
void SourcesDialog::load() {
  csm.clear();
  cl->clear();
  basis->readSourceList();
  append(basis->getSources());
}

/*! Adds a container to the widget, loads its data. */
void SourcesDialog::add() {
  QString p = QFileDialog::getOpenFileName(
      nullptr, QTranslator::tr("Select database"), nullptr,
      QTranslator::tr("ASW database") + "(*.asw.db)");
  if (p.isEmpty()) return;
  edited = true;
  append(basis->sql->sources(p));
}

/*! Removes selected containers. */
void SourcesDialog::remove() {
  if (!cl->selectedItems().length()) return;
  edited = true;
  for (int i = 0; i < cl->selectedItems().length(); i++) {
    QTreeWidgetItem *parent;
    if (!cl->selectedItems().at(i)->parent())
      parent = cl->invisibleRootItem();
    else
      parent = cl->selectedItems().at(i)->parent();
    parent->removeChild(cl->selectedItems().at(i));
    if (!parent->childCount() && (parent != cl->invisibleRootItem()))
      cl->invisibleRootItem()->removeChild(parent);
  }
}

/*! Establishes communications for user interaction through the dialog box. */
void SourcesDialog::connector() {
  connect(addBtn, &AButton::clicked, this, &SourcesDialog::add);
  connect(crtAct, &QAction::triggered, this, &SourcesDialog::openCC);
  connect(remAct, &QAction::triggered, this, &SourcesDialog::remove);
  connect(snclBtn, &AButton::clicked, this, &SourcesDialog::sncl);
}

/*!
 * Argument: QList of containers {cProps}.
 * Adds containers to the selection.
 */
void SourcesDialog::append(const QList<Source> &cProps) {
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

/*!
 * Argument: single container {cProp}.
 * Adds container to the selection.
 */
void SourcesDialog::appendSingle(const Source &cProp) {
  QList<Source> cProps;
  cProps.append(cProp);
  append(cProps);
  edited = true;
}

/*! Saves the selection and closes the dialog. */
void SourcesDialog::sncl() {
  if (edited) {
    QList<Source> cProps;
    for (int tli2 = 0; tli2 < cl->topLevelItemCount(); tli2++)
      for (int childIndex = 0;
           childIndex < cl->invisibleRootItem()->child(tli2)->childCount();
           childIndex++)
        cProps.append(
            csm.value(cl->invisibleRootItem()->child(tli2)->child(childIndex)));
    basis->writeSourceList(cProps);
  }
  close();
}

/*! Opens the container creation dialog. */
void SourcesDialog::openCC() {
  disconnect(crtAct, &QAction::triggered, this, &SourcesDialog::openCC);
  _cc = new CreateSourceDialog(basis, this);
  lt->addWidget(_cc, 2, 0, 1, 0);
  connect(_cc, &CreateSourceDialog::add, this, &SourcesDialog::appendSingle);
  connect(_cc, &CreateSourceDialog::cancelled, this, &SourcesDialog::closeCC);
}

/*! Closes the container creation dialog. */
void SourcesDialog::closeCC() {
  disconnect(_cc, &CreateSourceDialog::add, this, &SourcesDialog::appendSingle);
  disconnect(_cc, &CreateSourceDialog::cancelled, this, &SourcesDialog::closeCC);
  lt->removeWidget(_cc);
  connect(crtAct, &QAction::triggered, this, &SourcesDialog::openCC);
}
