#include "expression-editor.h"

/*! 
 * @brief The constructor. 
 * @details Selector layout scheme:
 * <--------------->
 * {   Select db   }
 * { Select  table }
 * [Expression list]
 * <      Tip      >
 * [ New ] [ Close ]
 * <--------------->
 * @details Brief layout scheme:
 * <--------------->
 * {Expression text}
 * [ Edit this one ]
 * [ Pre- ][ Post- ]
 * [Add...][ Add...]
 * [    <  Back    ]
 * <--------------->
 */
ExpressionEditor::ExpressionEditor(Basis *_basis, QWidget *parent, ModalHandler *m_handler)
    : ScrollFreezerWidget(parent), basis(_basis), _m_handler(m_handler) {
  _m_handler->setPrisoner(this);
  // Sets up selector widget.
  expressions.setHeaderLabels({tr("Address"), tr("Expression")});
  expressions.setWordWrap(true);
  expressions.setContextMenuPolicy(Qt::CustomContextMenu);
  connect(&expressions, &List::customContextMenuRequested, this,
          &ExpressionEditor::show_selector_expr_context_menu);
  connect(&expressions, &List::itemDoubleClicked, this, &ExpressionEditor::open_brief);
  connect(basis, &Basis::sources_changed, this, &ExpressionEditor::fill_databases);
  double_click_explain.setText(tr("Double click on the desired expression to edit it."));
  new_expression.setText(tr("New expression"));
  new_expression.setIcon(
    QIcon::fromTheme("list-add", QIcon(":/arts/icons/16/list-add.svg")));
  connect(&new_expression, &Button::clicked, this, &ExpressionEditor::add_new_expression);
  close_editor.setText(tr("Close"));
  close_editor.setIcon(
    QIcon::fromTheme("dialog-ok-apply", QIcon(":/arts/icons/16/dialog-ok-apply.svg")));
  connect(&close_editor, &Button::clicked, this, [this] { _m_handler->closePrisoner(); });
  selector_layout.setSpacing(0);
  selector_layout.setMargin(0);
  selector_layout.addWidget(&databases, 0, 0, 1, 0);
  selector_layout.addWidget(&tables, 1, 0, 1, 0);
  selector_layout.addWidget(&expressions, 2, 0, 1, 0);
  selector_layout.addWidget(&double_click_explain, 3, 0, 1, 0);
  selector_layout.addWidget(&new_expression, 4, 0);
  selector_layout.addWidget(&close_editor, 4, 1);
  selector.setLayout(&selector_layout);
  edit_expression_action.setText(tr("Edit this expression..."));
  edit_expression_action.setIcon(
    QIcon::fromTheme("edit", QIcon(":/arts/icons/16/document-edit.svg")));
  remove_expression_action.setText(tr("Delete this expression"));
  remove_expression_action.setIcon(
    QIcon::fromTheme("list-remove", QIcon(":/arts/icons/16/list-remove.svg")));
  expressions_context_menu.addAction(&edit_expression_action);
  expressions_context_menu.addAction(&remove_expression_action);
  // Sets up brief widget.
  auto brief_header_font = QApplication::font();
  brief_header_font.setPointSize(14);
  brief_header_font.setBold(true);
  brief_header.setFont(brief_header_font);
  brief_header.setWordWrap(true);
  accept_expression_text.setIcon(
    QIcon::fromTheme("dialog-ok-apply", QIcon(":/arts/icons/16/dialog-ok-apply.svg")));
  connect(&accept_expression_text, &Button::clicked, this, &ExpressionEditor::save_expression_text);
  expression_edit_line_layout.addWidget(&expression_edit);
  expression_edit_line_layout.addWidget(&accept_expression_text);
  back_to_selector.setText(tr("Back"));
  back_to_selector.setIcon(
    QIcon::fromTheme("go-previous", QIcon(":/arts/icons/16/go-previous.svg")));
  connect(&back_to_selector, &Button::clicked, this, &ExpressionEditor::close_brief);
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
void ExpressionEditor::fill_databases() {
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
void ExpressionEditor::fill_tables(const Sources &sources) {
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
  fill_expressions(sources);
  connect(&tables, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this, sources] {
    fill_expressions(sources);
  });
}

/*! @brief Gets the selected source from the widget. */
Source ExpressionEditor::selected_source() {
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

/*! @brief Populates a list of expressions from the source. */
void ExpressionEditor::fill_expressions(const Sources &sources) {
  current_sources = sources;
  selector_data = basis->sql->select_all(selected_source());
  QTreeWidgetItem *parent = expressions.invisibleRootItem();
  expressions.clear();
  for (auto pair : selector_data) {
    expressions.addTopLevelItem(new QTreeWidgetItem(parent, {QString::number(pair.first), pair.second}));
  }
}

/*! @brief Opens information about the expression. */
void ExpressionEditor::open_brief(QTreeWidgetItem *item, int column) {
  Q_UNUSED(column)
  if (not mode_mutex.try_lock()) return;
  brief_area.setFixedWidth(selector.width());
  brief_area.setFixedHeight(selector.height());
  selector.hide();
  editor_layout.replaceWidget(&selector, &brief_area);
  brief_header.setText(item->text(1));
  brief_area.show();
  mode = BriefMode;
  mode_mutex.unlock();
}

/*! @brief Returns to the list of expressions. */
void ExpressionEditor::close_brief() {
  if (not mode_mutex.try_lock()) return;
  brief_area.hide();
  editor_layout.replaceWidget(&brief_area, &selector);
  selector.show();
  mode = SelectorMode;
  mode_mutex.unlock();
}

/*! @brief Displays the context menu of the list of expressions. */
void ExpressionEditor::show_selector_expr_context_menu(const QPoint &pos) {
  QTreeWidgetItem *selected_item = expressions.itemAt(pos);
  disconnect(&edit_expression_action);
  disconnect(&remove_expression_action);
  connect(&edit_expression_action, &QAction::triggered, this, [this, selected_item] {
    open_brief(selected_item, 0);
  });
  expressions_context_menu.exec(QCursor::pos());
}

/*! @brief Adds a new expression and opens the editor. */
void ExpressionEditor::add_new_expression() {
  auto id = basis->sql->create_new_expression(selected_source(), tr("New expression"));
  if (id == -1) return;
  auto *expression_item = new QTreeWidgetItem(
    expressions.invisibleRootItem(), {QString::number(id), tr("New expression")});
  expressions.addTopLevelItem(expression_item);
  open_brief(expression_item, 0);
}

/*! @brief Saves new text of an expression. */
void ExpressionEditor::save_expression_text() {
  
}
