#include "sources.h"

/*
 * All short named objects and their explanations:
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
SourcesDialog::SourcesDialog(Basis *_basis, QWidget *parent,
                             ModalHandler *m_handler)
    : QWidget(parent), basis(_basis), _m_handler(m_handler) {
  _m_handler->setPrisoner(this);
  setObjectName(objn);
  grid_layout = new QGridLayout();
  grid_layout->setSpacing(0);
  grid_layout->setMargin(0);
  add_source = new AButton(QTranslator::tr("Add source"), this);
  create_source = new QAction(QTranslator::tr("Create"), this);
  remove_source = new QAction(QTranslator::tr("Remove"), this);
  save_and_close = new AButton(QTranslator::tr("Save and close"), this);
  add_source->setIcon(QIcon(":/arts/icons/16/insert-link.svg"));
  create_source->setIcon(QIcon(":/arts/icons/16/document-new.svg"));
  remove_source->setIcon(QIcon(":/arts/icons/16/remove-link.svg"));
  save_and_close->setIcon(QIcon(":/arts/icons/16/dialog-ok-apply.svg"));
  add_source->setPopupMode(QToolButton::MenuButtonPopup);
  add_source->addAction(create_source);
  add_source->addAction(remove_source);
  source_list = new ASourceList(this);
  grid_layout->addWidget(source_list, 0, 0, 1, 0);
  grid_layout->addWidget(add_source, 1, 0);
  grid_layout->addWidget(save_and_close, 1, 1);
  setLayout(grid_layout);
  connector();
  load();
}

/*! Loads saved sources. */
void SourcesDialog::load() {
  source_widgets.clear();
  source_list->clear();
  basis->read_source_list();
  append(basis->get_sources());
}

/*! Adds a source to the widget, loads its data. */
void SourcesDialog::add() {
  QString filename = QFileDialog::getOpenFileName(
      nullptr, QTranslator::tr("Select database"), nullptr,
      QTranslator::tr("ASW database") + "(*.asw.db)");
  if (filename.isEmpty())
    return;
  edited = true;
  append(basis->sql->sources(filename));
}

/*! Removes selected sources. */
void SourcesDialog::remove() {
  if (not source_list->selectedItems().length())
    return;
  edited = true;
  for (auto *item : source_list->selectedItems()) {
    QTreeWidgetItem *parent;
    if (not item->parent())
      parent = source_list->invisibleRootItem();
    else
      parent = item->parent();
    parent->removeChild(item);
    if (not parent->childCount() and
        (parent != source_list->invisibleRootItem()))
      source_list->invisibleRootItem()->removeChild(parent);
  }
}

/*! Establishes communications for user interaction through the dialog box. */
void SourcesDialog::connector() {
  connect(add_source, &AButton::clicked, this, &SourcesDialog::add);
  connect(create_source, &QAction::triggered, this, &SourcesDialog::openCS);
  connect(remove_source, &QAction::triggered, this, &SourcesDialog::remove);
  connect(save_and_close, &AButton::clicked, this, &SourcesDialog::sncl);
}

/*!
 * Argument: QList of sources {sources}.
 * Adds sources to the selection.
 */
void SourcesDialog::append(const QList<Source> &sources) {
  for (const auto &source : sources) {
    QTreeWidgetItem *parent = nullptr;
    bool isInside = false;
    for (int tli2 = 0; tli2 < source_list->topLevelItemCount(); tli2++)
      if (source_list->invisibleRootItem()->child(tli2)->text(0) ==
          source.path) {
        isInside = true;
        parent = source_list->invisibleRootItem()->takeChild(tli2);
        break;
      }
    if (not isInside)
      parent = new QTreeWidgetItem(QStringList(source.path));
    source_list->addTopLevelItem(parent);
    bool notContains = true;
    for (int childIndex = 0; childIndex < parent->childCount(); childIndex++)
      if (parent->child(childIndex)->text(0) == source.table_title) {
        notContains = false;
        break;
      }
    if (notContains) {
      auto *cell = new QTreeWidgetItem(parent, QStringList(source.table_title));
      source_widgets.insert(cell, source);
    }
  }
}

/*!
 * Argument: single source {source}.
 * Adds source to the selection.
 */
void SourcesDialog::appendSingle(const Source &source) {
  QList<Source> sources;
  sources.append(source);
  append(sources);
  edited = true;
}

/*! Saves the selection and closes the dialog. */
void SourcesDialog::sncl() {
  if (edited) {
    QList<Source> sources;
    for (int tli2 = 0; tli2 < source_list->topLevelItemCount(); tli2++)
      for (int childIndex = 0;
           childIndex <
           source_list->invisibleRootItem()->child(tli2)->childCount();
           childIndex++)
        sources.append(source_widgets.value(
            source_list->invisibleRootItem()->child(tli2)->child(childIndex)));
    basis->write_source_list(sources);
  }
  _m_handler->closePrisoner();
}

/*! Opens the source creation dialog. */
void SourcesDialog::openCS() {
  disconnect(create_source, &QAction::triggered, this, &SourcesDialog::openCS);
  create_source_dialog = new CreateSourceDialog(basis, this);
  grid_layout->addWidget(create_source_dialog, 2, 0, 1, 0);
  connect(create_source_dialog, &CreateSourceDialog::add, this,
          &SourcesDialog::appendSingle);
  connect(create_source_dialog, &CreateSourceDialog::cancelled, this,
          &SourcesDialog::closeCS);
}

/*! Closes the source creation dialog. */
void SourcesDialog::closeCS() {
  disconnect(create_source_dialog, &CreateSourceDialog::add, this,
             &SourcesDialog::appendSingle);
  disconnect(create_source_dialog, &CreateSourceDialog::cancelled, this,
             &SourcesDialog::closeCS);
  grid_layout->removeWidget(create_source_dialog);
  connect(create_source, &QAction::triggered, this, &SourcesDialog::openCS);
}
