#include "sources.h"

/*!
 * @details All short named objects and their explanations:
 * @a tli2 <- top-level item index <- @a SourcesDialog::append,
 *                                    @a SourcesDialog::sncl
 */

/*!
 * @brief The constructor.
 * @details Layout scheme:
 * <----------------------->
 * [     Sources  list     ]
 * [Add button][S&C  button]
 * {|> Create }
 * {|> Remove }
 * <----------------------->
 */
SourcesDialog::SourcesDialog(Basis *_basis, QWidget *parent, ModalHandler *m_handler)
    : QWidget(parent), basis(_basis), _m_handler(m_handler) {
  _m_handler->setPrisoner(this);
  setObjectName(object_name);
  grid_layout = new QGridLayout();
  grid_layout->setSpacing(0);
  grid_layout->setMargin(0);
  source_actions = new Button(tr("Source actions..."), this);
  add_source = new QAction(tr("Add sources"), this);
  create_source = new QAction(tr("Create source"), this);
  remove_source = new QAction(tr("Remove source"), this);
  save_and_close = new Button(tr("Save and close"), this);
  source_actions->setIcon(
    QIcon::fromTheme("insert-link", QIcon(":/arts/icons/16/insert-link.svg")));
  add_source->setIcon(
    QIcon::fromTheme("insert-link", QIcon(":/arts/icons/16/insert-link.svg")));
  create_source->setIcon(
    QIcon::fromTheme("document-new", QIcon(":/arts/icons/16/document-new.svg")));
  remove_source->setIcon(
    QIcon::fromTheme("remove-link", QIcon(":/arts/icons/16/remove-link.svg")));
  save_and_close->setIcon(
    QIcon::fromTheme("dialog-ok-apply", QIcon(":/arts/icons/16/dialog-ok-apply.svg")));
  auto *add_source_menu = new Menu(source_actions);
  add_source_menu->addAction(add_source);
  add_source_menu->addAction(create_source);
  add_source_menu->addAction(remove_source);
  source_actions->setMenu(add_source_menu);
  source_list = new List(this);
  source_list->setHeaderLabel(tr("Sources"));
  grid_layout->addWidget(source_list, 0, 0, 1, 0);
  grid_layout->addWidget(source_actions, 1, 0);
  grid_layout->addWidget(save_and_close, 1, 1);
  setLayout(grid_layout);
  setFixedWidth(480);
  connector();
  load();
}

/*! @brief Loads saved sources. */
void SourcesDialog::load() {
  source_widgets.clear();
  source_list->clear();
  basis->load_sources();
  append(basis->sources());
}

/*! @brief Adds a source to the widget, loads its data. */
void SourcesDialog::add() {
  QString filename = QFileDialog::getOpenFileName(
    nullptr, tr("Select database"), nullptr, tr("Jeff's database") + "(*.j.db)"
  );
  if (filename.isEmpty()) return;
  edited = true;
  append(basis->sql->sources(filename));
}

/*! @brief Removes selected sources. */
void SourcesDialog::remove() {
  if (not source_list->selectedItems().length()) return;
  edited = true;
  for (auto *item : source_list->selectedItems()) {
    QTreeWidgetItem *parent;
    if (not item->parent()) parent = source_list->invisibleRootItem();
    else parent = item->parent();
    parent->removeChild(item);
    if (not parent->childCount() and (parent != source_list->invisibleRootItem()))
      source_list->invisibleRootItem()->removeChild(parent);
  }
}

/*! @brief Establishes communications for user interaction through the dialog box. */
void SourcesDialog::connector() {
  connect(add_source, &QAction::triggered, this, &SourcesDialog::add);
  connect(create_source, &QAction::triggered, this, &SourcesDialog::openCS);
  connect(remove_source, &QAction::triggered, this, &SourcesDialog::remove);
  connect(save_and_close, &Button::clicked, this, &SourcesDialog::sncl);
}

/*! @brief Adds sources to the selection. */
void SourcesDialog::append(Sources sources) {
  for (const auto &source : sources) {
    QTreeWidgetItem *parent = nullptr;
    bool isInside = false;
    for (int tli2 = 0; tli2 < source_list->topLevelItemCount(); tli2++)
      if (source_list->invisibleRootItem()->child(tli2)->text(0) == source.path) {
        isInside = true;
        parent = source_list->invisibleRootItem()->takeChild(tli2);
        break;
      }
    if (not isInside) parent = new QTreeWidgetItem(QStringList(source.path));
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

/*!@brief Adds source to the selection. */
void SourcesDialog::appendSingle(const Source &source) {
  QList<Source> sources;
  sources.append(source);
  append(sources);
  edited = true;
}

/*! @brief Saves the selection and closes the dialog. */
void SourcesDialog::sncl() {
  if (edited) {
    Sources sources;
    for (int i = 0; i < source_list->topLevelItemCount(); i++)
      for (int j = 0; j < source_list->invisibleRootItem()->child(i)->childCount(); j++)
        sources.append(source_widgets.value(source_list->invisibleRootItem()->child(i)->child(j)));
    basis->sources(sources);
  }
  _m_handler->closePrisoner();
}

/*! @brief Opens the source creation dialog. */
void SourcesDialog::openCS() {
  disconnect(create_source, &QAction::triggered, this, &SourcesDialog::openCS);
  create_source_dialog = new CreateSourceDialog(basis, this);
  grid_layout->addWidget(create_source_dialog, 2, 0, 1, 0);
  connect(create_source_dialog, &CreateSourceDialog::add, this, &SourcesDialog::appendSingle);
  connect(create_source_dialog, &CreateSourceDialog::cancelled, this, &SourcesDialog::closeCS);
}

/*! @brief Closes the source creation dialog. */
void SourcesDialog::closeCS() {
  disconnect(create_source_dialog, &CreateSourceDialog::add, this, &SourcesDialog::appendSingle);
  disconnect(create_source_dialog, &CreateSourceDialog::cancelled, this, &SourcesDialog::closeCS);
  grid_layout->removeWidget(create_source_dialog);
  connect(create_source, &QAction::triggered, this, &SourcesDialog::openCS);
}
