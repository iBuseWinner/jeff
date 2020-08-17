#include "sources.h"

/*!
 * @details All short named objects and their explanations:
 * @a tli2 <- top-level item index <- @a SourcesDialog::append,
 *                                    @a SourcesDialog::sncl
 */

/*!
 * @fn SourcesDialog::SourcesDialog
 * @brief The constructor.
 * @details Layout scheme:
 * <----------------------->
 * [     Sources  list     ]
 * [Add button][S&C  button]
 * {|> Create }
 * {|> Remove }
 * <----------------------->
 * @param[in,out] _basis reference to the Basis instance
 * @param[in,out] parent QObject parent
 * @param[in,out] m_handler reference to the ModalHandler instance
 */
SourcesDialog::SourcesDialog(Basis *_basis, QWidget *parent,
                             ModalHandler *m_handler)
    : QWidget(parent), basis(_basis), _m_handler(m_handler) {
  _m_handler->setPrisoner(this);
  setObjectName(object_name);
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

/*!
 * @fn SourcesDialog::load
 * @brief Loads saved sources.
 */
void SourcesDialog::load() {
  source_widgets.clear();
  source_list->clear();
  basis->read_source_list();
  append(basis->get_sources());
}

/*!
 * @fn SourcesDialog::add
 * @brief Adds a source to the widget, loads its data.
 */
void SourcesDialog::add() {
  QString filename = QFileDialog::getOpenFileName(
      nullptr, QTranslator::tr("Select database"), nullptr,
      QTranslator::tr("ASW database") + "(*.asw.db)");
  if (filename.isEmpty())
    return;
  edited = true;
  append(basis->sql->sources(filename));
}

/*!
 * @fn SourcesDialog::remove
 * @brief Removes selected sources.
 */
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

/*!
 * @fn SourcesDialog::connector
 * @brief Establishes communications for user interaction through the dialog
 * box.
 */
void SourcesDialog::connector() {
  connect(add_source, &AButton::clicked, this, &SourcesDialog::add);
  connect(create_source, &QAction::triggered, this, &SourcesDialog::openCS);
  connect(remove_source, &QAction::triggered, this, &SourcesDialog::remove);
  connect(save_and_close, &AButton::clicked, this, &SourcesDialog::sncl);
}

/*!
 * @fn SourcesDialog::append
 * @brief Adds sources to the selection.
 * @param[in] sources sources to be added
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
 * @fn SourcesDialog::appendSingle
 * @brief Adds source to the selection.
 * @param[in] source source data
 */
void SourcesDialog::appendSingle(const Source &source) {
  QList<Source> sources;
  sources.append(source);
  append(sources);
  edited = true;
}

/*!
 * @fn SourcesDialog::sncl
 * @brief Saves the selection and closes the dialog.
 */
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

/*!
 * @fn SourcesDialog::openCS
 * @brief Opens the source creation dialog.
 */
void SourcesDialog::openCS() {
  disconnect(create_source, &QAction::triggered, this, &SourcesDialog::openCS);
  create_source_dialog = new CreateSourceDialog(basis, this);
  grid_layout->addWidget(create_source_dialog, 2, 0, 1, 0);
  connect(create_source_dialog, &CreateSourceDialog::add, this,
          &SourcesDialog::appendSingle);
  connect(create_source_dialog, &CreateSourceDialog::cancelled, this,
          &SourcesDialog::closeCS);
}

/*!
 * @fn SourcesDialog::closeCS
 * @brief Closes the source creation dialog.
 */
void SourcesDialog::closeCS() {
  disconnect(create_source_dialog, &CreateSourceDialog::add, this,
             &SourcesDialog::appendSingle);
  disconnect(create_source_dialog, &CreateSourceDialog::cancelled, this,
             &SourcesDialog::closeCS);
  grid_layout->removeWidget(create_source_dialog);
  connect(create_source, &QAction::triggered, this, &SourcesDialog::openCS);
}
