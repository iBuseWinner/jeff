#include "overview.hpp"

/*! @brief The constructor. */
PhraseEditorOverview::PhraseEditorOverview(Basis *_basis, QWidget *parent)
    : QWidget(parent), basis(_basis) {
  phrases_list.setHeaderLabels({tr("Address"), tr("Phrase")});
  phrases_list.setWordWrap(true);
  phrases_list.setContextMenuPolicy(Qt::CustomContextMenu);
  connect(&phrases_list, &List::customContextMenuRequested, this, &PhraseEditorOverview::show_context_menu);
  connect(basis, &Basis::sources_changed, this, &PhraseEditorOverview::fill_databases);
  double_click_explain.setText(tr("Double click on the desired phrase to edit it."));
  new_phrase.setText(tr("New phrase"));
  new_phrase.setIcon(QIcon::fromTheme("list-add", QIcon(":/arts/icons/16/list-add.svg")));
  close_editor.setText(tr("Close"));
  close_editor.setIcon(QIcon::fromTheme("collapse-all", QIcon(":/arts/icons/16/collapse-all.svg")));
  overview_layout.setSpacing(0);
  overview_layout.setMargin(0);
  overview_layout.addWidget(&databases, 0, 0, 1, 0);
  overview_layout.addWidget(&tables, 1, 0, 1, 0);
  overview_layout.addWidget(&phrases_list, 2, 0, 1, 0);
  overview_layout.addWidget(&double_click_explain, 3, 0, 1, 0);
  overview_layout.addWidget(&new_phrase, 4, 0);
  overview_layout.addWidget(&close_editor, 4, 1);
  edit_phrase_action.setText(tr("Edit this phrase..."));
  edit_phrase_action.setIcon(QIcon::fromTheme("edit", QIcon(":/arts/icons/16/document-edit.svg")));
  remove_phrase_action.setText(tr("Delete this phrase"));
  remove_phrase_action.setIcon(QIcon::fromTheme("list-remove", QIcon(":/arts/icons/16/list-remove.svg")));
  phrase_context_menu.addAction(&edit_phrase_action);
  phrase_context_menu.addAction(&remove_phrase_action);
  setLayout(&overview_layout);
}

/*! @brief Displays the context menu of the list of phrases. */
void PhraseEditorOverview::show_context_menu(const QPoint &pos) {
  auto *selected_item = phrases_list.itemAt(pos);
  if (not selected_item) return;
  edit_phrase_action.disconnect();
  remove_phrase_action.disconnect();
  connect(&edit_phrase_action, &QAction::triggered, this, [this, selected_item] {
    emit open_brief(selected_item);
  });
  connect(&remove_phrase_action, &QAction::triggered, this, [this, selected_item] {
    auto address = selected_item->text(0).toInt();
    if (not basis->sql->remove_phrase(selected_source(), address)) return;
    for (int i = 0; i < phrases.length(); i++) {
      if (phrases[i].links.contains(address)) {
        phrases[i].links.remove(address);
        basis->sql->update_links(selected_source(), phrases[i].links, phrases[i].address);
      }
    }
    auto *item = phrases_list.takeTopLevelItem(phrases_list.indexOfTopLevelItem(selected_item));
    if (item) delete item;
  });
  phrase_context_menu.exec(QCursor::pos());
}

/*! @brief Populates the list of databases. */
void PhraseEditorOverview::fill_databases() {
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
void PhraseEditorOverview::fill_tables(const Sources &sources) {
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
  fill_phrases();
  connect(&tables, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this, sources] {
    fill_phrases();
  });
}

/*! @brief Gets the selected source from the widget. */
Source PhraseEditorOverview::selected_source() {
  Source selected;
  selected.path = databases.currentData().toString();
  selected.table_name = tables.currentData().toString();
  return selected;
}

/*! @brief @sa @a PhraseEditorOverview::fill_phrases */
void PhraseEditorOverview::update_phrases() { fill_phrases(); }

/*! @brief Populates a list of phrases from the source. */
void PhraseEditorOverview::fill_phrases() {
  phrases = basis->sql->select_all(selected_source());
  QTreeWidgetItem *parent = phrases_list.invisibleRootItem();
  phrases_list.clear();
  for (auto phrase : phrases) {
    phrases_list.addTopLevelItem(
      new QTreeWidgetItem(parent, {QString::number(phrase.address), phrase.phrase})
    );
  }
}
