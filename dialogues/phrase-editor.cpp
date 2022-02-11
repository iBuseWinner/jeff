#include "phrase-editor.h"

/*! 
 * @brief The constructor. 
 * @details Selector layout scheme:
 * <--------------->
 * {   Select db   }
 * { Select  table }
 * [  Phrase list  ]
 * <      Tip      >
 * [ New ] [ Close ]
 * <--------------->
 * @details Brief layout scheme:
 * <--------------->
 * {  Phrase text  }
 * [ Edit this one ]
 * [ Pre- ][ Post- ]
 * [Add...][ Add...]
 * [    <  Back    ]
 * <--------------->
 */
PhraseEditor::PhraseEditor(Basis *_basis, QWidget *parent, ModalHandler *m_handler)
    : ScrollFreezerWidget(parent), basis(_basis), _m_handler(m_handler) {
  _m_handler->setPrisoner(this);
  // Sets up selector widget.
  phrases.setHeaderLabels({tr("Address"), tr("Expression")});
  phrases.setWordWrap(true);
  phrases.setContextMenuPolicy(Qt::CustomContextMenu);
  connect(&phrases, &List::customContextMenuRequested, this,
          &PhraseEditor::show_selector_phrase_context_menu);
  connect(&phrases, &List::itemDoubleClicked, this, &PhraseEditor::open_brief);
  connect(basis, &Basis::sources_changed, this, &PhraseEditor::fill_databases);
  double_click_explain.setText(tr("Double click on the desired phrase to edit it."));
  new_phrase.setText(tr("New phrase"));
  new_phrase.setIcon(
    QIcon::fromTheme("list-add", QIcon(":/arts/icons/16/list-add.svg")));
  connect(&new_phrase, &Button::clicked, this, &PhraseEditor::add_new_phrase);
  close_editor.setText(tr("Close"));
  close_editor.setIcon(
    QIcon::fromTheme("dialog-ok-apply", QIcon(":/arts/icons/16/dialog-ok-apply.svg")));
  connect(&close_editor, &Button::clicked, this, [this] { _m_handler->closePrisoner(); });
  selector_layout.setSpacing(0);
  selector_layout.setMargin(0);
  selector_layout.addWidget(&databases, 0, 0, 1, 0);
  selector_layout.addWidget(&tables, 1, 0, 1, 0);
  selector_layout.addWidget(&phrases, 2, 0, 1, 0);
  selector_layout.addWidget(&double_click_explain, 3, 0, 1, 0);
  selector_layout.addWidget(&new_phrase, 4, 0);
  selector_layout.addWidget(&close_editor, 4, 1);
  selector.setLayout(&selector_layout);
  edit_phrase_action.setText(tr("Edit this phrase..."));
  edit_phrase_action.setIcon(
    QIcon::fromTheme("edit", QIcon(":/arts/icons/16/document-edit.svg")));
  remove_phrase_action.setText(tr("Delete this phrase"));
  remove_phrase_action.setIcon(
    QIcon::fromTheme("list-remove", QIcon(":/arts/icons/16/list-remove.svg")));
  phrase_context_menu.addAction(&edit_phrase_action);
  phrase_context_menu.addAction(&remove_phrase_action);
  // Sets up brief widget.
  auto brief_header_font = QApplication::font();
  brief_header_font.setPointSize(14);
  brief_header_font.setBold(true);
  brief_header.setFont(brief_header_font);
  brief_header.setWordWrap(true);
  accept_phrase_text.setIcon(
    QIcon::fromTheme("dialog-ok-apply", QIcon(":/arts/icons/16/dialog-ok-apply.svg")));
  connect(&accept_phrase_text, &Button::clicked, this, &PhraseEditor::save_phrase_text);
  phrase_edit_line_layout.addWidget(&phrase_edit);
  phrase_edit_line_layout.addWidget(&accept_phrase_text);
  back_to_selector.setText(tr("Back"));
  back_to_selector.setIcon(
    QIcon::fromTheme("go-previous", QIcon(":/arts/icons/16/go-previous.svg")));
  connect(&back_to_selector, &Button::clicked, this, &PhraseEditor::close_brief);
  brief_area_layout.setSpacing(0);
  brief_area_layout.setMargin(0);
  brief_area_layout.addWidget(&brief_header, 0, 0);
  brief_area_layout.addItem(new QSpacerItem(0, 0, QSizePolicy::Fixed, QSizePolicy::Expanding), 1, 0);
  brief_area_layout.addWidget(&back_to_selector, 2, 0);
  brief_area_widget.setLayout(&brief_area_layout);
  brief_area.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  brief_area.setFocusPolicy(Qt::NoFocus);
  brief_area.setFrameStyle(QFrame::NoFrame);
  brief_area.setFrameShadow(QFrame::Plain);
  brief_area.setWidgetResizable(true);
  brief_area.setWidget(&brief_area_widget);
  brief_area.hide();
  // Shows selector.
  editor_layout.setSpacing(0);
  editor_layout.setMargin(0);
  editor_layout.addWidget(&selector);
  setLayout(&editor_layout);
  setFixedWidth(480);
  fill_databases();
}

/*! @brief Populates the list of databases. */
void PhraseEditor::fill_databases() {
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
void PhraseEditor::fill_tables(const Sources &sources) {
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
Source PhraseEditor::selected_source() {
  Source selected;
  for (auto candidate : current_sources) {
    if (
      candidate.path == databases.currentData().toString() and 
      candidate.table_name == tables.currentData().toString()
    ) {
      selected = candidate;
      break;
    }
  }
  return selected;
}

/*! @brief Populates a list of phrases from the source. */
void PhraseEditor::fill_phrases(const Sources &sources) {
  current_sources = sources;
  selector_data = basis->sql->select_all(selected_source());
  QTreeWidgetItem *parent = phrases.invisibleRootItem();
  phrases.clear();
  for (auto phrase : selector_data) {
    phrases.addTopLevelItem(
      new QTreeWidgetItem(parent, {QString::number(phrase.address), phrase.expression})
    );
  }
}

/*! @brief Opens information about the phrase. */
void PhraseEditor::open_brief(QTreeWidgetItem *item, int column) {
  Q_UNUSED(column)
  if (not mode_mutex.try_lock()) return;
  current_phrase = basis->sql->get_phrase_by_address(selected_source(), item->text(0).toInt());
  brief_area.setFixedWidth(selector.width());
  brief_area.setFixedHeight(selector.height());
  selector.hide();
  editor_layout.replaceWidget(&selector, &brief_area);
  brief_header.setText(current_phrase.expression);
  brief_area.show();
  mode = BriefMode;
  mode_mutex.unlock();
}

/*! @brief Returns to the list of expressions. */
void PhraseEditor::close_brief() {
  if (not mode_mutex.try_lock()) return;
  brief_area.hide();
  editor_layout.replaceWidget(&brief_area, &selector);
  selector.show();
  mode = SelectorMode;
  mode_mutex.unlock();
}

/*! @brief Displays the context menu of the list of phrases. */
void PhraseEditor::show_selector_phrase_context_menu(const QPoint &pos) {
  QTreeWidgetItem *selected_item = phrases.itemAt(pos);
  if (not selected_item) return;
  disconnect(&edit_phrase_action);
  disconnect(&remove_phrase_action);
  connect(&edit_phrase_action, &QAction::triggered, this, [this, selected_item] {
    open_brief(selected_item, 0);
  });
  phrase_context_menu.exec(QCursor::pos());
}

/*! @brief Adds a new phrase and opens the editor. */
void PhraseEditor::add_new_phrase() {
  auto id = basis->sql->create_new_phrase(selected_source(), tr("New phrase"));
  if (id == -1) return;
  auto *phrase_item = new QTreeWidgetItem(
    phrases.invisibleRootItem(), {QString::number(id), tr("New phrase")});
  phrases.addTopLevelItem(phrase_item);
  open_brief(phrase_item, 0);
}

/*! @brief Saves new text of an phrase. */
void PhraseEditor::save_phrase_text() {}
