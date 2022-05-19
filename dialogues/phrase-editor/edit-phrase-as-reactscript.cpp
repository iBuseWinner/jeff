#include "edit-phrase-as-reactscript.h"

/*! @brief The constructor. */
PhraseEditorEditAsReactScript::PhraseEditorEditAsReactScript(Basis *_basis, QWidget *parent)
    : QWidget(parent), basis(_basis) {
  choose_path.setText(tr("Specify script path:"));
  specify_func_name.setText(tr("Specify function name:"));
  specify_amount_of_history.setText(tr("Specify amount of message history to be sent:"));
  specify_amount_of_history.setWordWrap(true);
  needs_user_input.setText(tr("Check if script needs whole user input"));
  specify_memory_cells.setText(tr("Specify memory cells values to be sent:"));
  path.setText(tr("Select a file..."));
  connect(&path, &Button::clicked, this, &PhraseEditorEditAsReactScript::select_file);
  func_name.setPlaceholderText(tr("Function name..."));
  add_memory_cell_btn.setText(tr("Add memory cell"));
  remove_memory_cell_btn.setText(tr("Remove selected cell"));
  save_script_btn.setText(tr("Save script"));
  save_script_btn.setIcon(
    QIcon::fromTheme("dialog-ok-apply", QIcon(":/arts/icons/16/dialog-ok-apply.svg")));
  connect(&save_script_btn, &Button::clicked, this, &PhraseEditorEditAsReactScript::save_script);
  memory_cells.setHeaderLabels({tr("Memory cell key")});
  memory_cells_manager_layout.setSpacing(0);
  memory_cells_manager_layout.setMargin(0);
  memory_cells_manager_layout.addWidget(&memory_cells, 0, 0, 2, 1);
  memory_cells_manager_layout.addWidget(&add_memory_cell_btn, 0, 1);
  memory_cells_manager_layout.addWidget(&remove_memory_cell_btn, 1, 1);
  memory_cells_manager_widget.setContentsMargins(0, 0, 0, 0);
  memory_cells_manager_widget.setLayout(&memory_cells_manager_layout);
  other_props_widget_layout.setSpacing(0);
  other_props_widget_layout.setMargin(0);
  other_props_widget_layout.addWidget(&specify_memory_cells, 0, 0);
  other_props_widget_layout.addWidget(&memory_cells_manager_widget, 1, 0);
  other_props_widget.set_title(tr("Expand other properties"));
  other_props_widget.set_content_layout(&other_props_widget_layout);
  editor_layout.setMargin(0);
  editor_layout.addWidget(&choose_path, 0, 0);
  editor_layout.addWidget(&path, 0, 1);
  editor_layout.addWidget(&specify_func_name, 1, 0);
  editor_layout.addWidget(&func_name, 1, 1);
  editor_layout.addWidget(&specify_amount_of_history, 2, 0);
  editor_layout.addWidget(&history_amount, 2, 1);
  editor_layout.addWidget(&needs_user_input, 3, 0, 1, 2);
  editor_layout.addWidget(&other_props_widget, 4, 0, 1, 2);
  editor_layout.addWidget(&save_script_btn, 5, 0, 1, 2);
  setLayout(&editor_layout);
}

/*! @brief The destructor. */
PhraseEditorEditAsReactScript::~PhraseEditorEditAsReactScript() {
  if (script) delete script;
}

/*! @brief TBD */
void PhraseEditorEditAsReactScript::load_from_text(QString expression) {
  if (expression.isEmpty()) return;
  auto *meta_script = ScriptsCast::to_script(expression);
  if (not meta_script) return;
  if (script) delete script;
  script = dynamic_cast<ReactScript *>(meta_script);
  if (not script) return;
  path.setText(script->path);
  func_name.setText(script->fn_name);
  history_amount.setValue(script->number_of_hist_messages);
  needs_user_input.setChecked(script->needs_user_input);
  for (auto memory_cell : script->memory_cells) {
    auto *item = new QTreeWidgetItem(QStringList(memory_cell));
    memory_cells_items.append(item);
    memory_cells.addTopLevelItem(item);
  }
}

/*! @brief TBD */
void PhraseEditorEditAsReactScript::clear() {
  path.setText(tr("Select a file..."));
  func_name.setText("");
  history_amount.setValue(0);
  memory_cells.clear();
  needs_user_input.setChecked(false);
  for (auto *item : memory_cells_items) if (item) delete item;
  memory_cells_items.clear();
}

/*! @brief TBD */
void PhraseEditorEditAsReactScript::select_file() {
  auto _path = QFileDialog::getOpenFileName(nullptr, tr("Select file..."), "", tr("Python script") + "(*.py)");
  if (not _path.isEmpty()) {
    path.setText(_path);
  } else path.setText(tr("Select a file..."));
}

/*! @brief TBD */
void PhraseEditorEditAsReactScript::save_script() {
  if (not script) script = new ReactScript();
  if (path.text() != tr("Select a file...")) script->path = path.text();
  else script->path = "";
  script->stype = ScriptType::React;
  script->fn_name = func_name.text();
  script->number_of_hist_messages = history_amount.value();
  script->needs_user_input = needs_user_input.isChecked();
  for (auto *item : memory_cells_items) script->memory_cells.append(item->text(0));
  clear();
  emit save(ScriptsCast::to_string(script));
}
