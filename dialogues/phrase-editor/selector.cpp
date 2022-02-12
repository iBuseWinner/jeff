#include "selector.h"

/*! @brief The constructor. */
PhraseEditorSelector::PhraseEditorSelector(Basis *_basis, QWidget *parent)
    : QWidget(parent), basis(_basis) {
  phrases_list.setHeaderLabels({tr("Address"), tr("Phrase")});
  phrases_list.setWordWrap(true);
  phrases_list.setContextMenuPolicy(Qt::CustomContextMenu);
  connect(&phrases_list, &List::customContextMenuRequested, this, &PhraseEditorSelector::show_context_menu);
  connect(basis, &Basis::sources_changed, this, &PhraseEditorSelector::fill_databases);
  double_click_explain.setText(tr("Double click on the desired phrase to edit it."));
  new_phrase.setText(tr("New phrase"));
  new_phrase.setIcon(
    QIcon::fromTheme("list-add", QIcon(":/arts/icons/16/list-add.svg")));
  close_editor.setText(tr("Close"));
  close_editor.setIcon(
    QIcon::fromTheme("dialog-ok-apply", QIcon(":/arts/icons/16/dialog-ok-apply.svg")));
  selector_layout.setSpacing(0);
  selector_layout.setMargin(0);
  selector_layout.addWidget(&databases, 0, 0, 1, 0);
  selector_layout.addWidget(&tables, 1, 0, 1, 0);
  selector_layout.addWidget(&phrases_list, 2, 0, 1, 0);
  selector_layout.addWidget(&double_click_explain, 3, 0, 1, 0);
  selector_layout.addWidget(&new_phrase, 4, 0);
  selector_layout.addWidget(&close_editor, 4, 1);
  edit_phrase_action.setText(tr("Edit this phrase..."));
  edit_phrase_action.setIcon(
    QIcon::fromTheme("edit", QIcon(":/arts/icons/16/document-edit.svg")));
  remove_phrase_action.setText(tr("Delete this phrase"));
  remove_phrase_action.setIcon(
    QIcon::fromTheme("list-remove", QIcon(":/arts/icons/16/list-remove.svg")));
  phrase_context_menu.addAction(&edit_phrase_action);
  phrase_context_menu.addAction(&remove_phrase_action);
  setLayout(&selector_layout);
}

/*! @brief Displays the context menu of the list of phrases. */
void PhraseEditorSelector::show_context_menu(const QPoint &pos) {
  QTreeWidgetItem *selected_item = phrases_list.itemAt(pos);
  if (not selected_item) return;
  disconnect(&edit_phrase_action);
  disconnect(&remove_phrase_action);
  connect(&edit_phrase_action, &QAction::triggered, this, [this, selected_item] {
    emit open_brief(selected_item);
  });
  phrase_context_menu.exec(QCursor::pos());
}

/*! @brief Populates the list of databases. */
void PhraseEditorSelector::fill_databases() {
  disconnect(&databases, QOverload<int>::of(&QComboBox::currentIndexChanged), nullptr, nullptr);
  auto selected = databases.currentData();
  auto sources = basis->sources();
  for (auto source : sources) {
    if (databases.findData(source.path) == -1)
      databases.addItem(
        styling.metrics->elidedText(source.path, Qt::ElideLeft, width() - 30), source.path);
  }
  if (selected.isValid()) {
    int id = -1;
    if ((id = databases.findData(selected)) != -1)
      databases.setCurrentIndex(id);
  } else fill_tables(sources);
  connect(&databases, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this, sources] {
    fill_tables(sources);
  });
}

/*! @brief Populates the list of tables in the database. */
void PhraseEditorSelector::fill_tables(const Sources &sources) {
  disconnect(&tables, QOverload<int>::of(&QComboBox::currentIndexChanged), nullptr, nullptr);
  auto selected = tables.currentData();
  for (auto source : sources) {
    if (tables.findData(source.table_name) == -1)
      tables.addItem(
        styling.metrics->elidedText(source.table_title, Qt::ElideLeft, width() - 30), source.table_name);
  }
  if (selected.isValid()) {
    int id = -1;
    if ((id = tables.findData(selected)) != -1)
      tables.setCurrentIndex(id);
  }
  fill_phrases(sources);
  connect(&tables, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this, sources] {
    fill_phrases(sources);
  });
}

/*! @brief Gets the selected source from the widget. */
Source PhraseEditorSelector::selected_source() {
  Source selected;
  selected.path = databases.currentData().toString();
  selected.table_name = tables.currentData().toString();
  return selected;
}

/*! @brief Populates a list of phrases from the source. */
void PhraseEditorSelector::fill_phrases(const Sources &sources) {
  auto selector_data = basis->sql->select_all(selected_source());
  QTreeWidgetItem *parent = phrases_list.invisibleRootItem();
  phrases_list.clear();
  for (auto phrase : selector_data) {
    phrases_list.addTopLevelItem(
      new QTreeWidgetItem(parent, {QString::number(phrase.address), phrase.expression})
    );
  }
}
