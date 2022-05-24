#include "edit-phrase-as-reactscript.h"

/*! @brief The constructor. */
PhraseEditorEditAsReactScript::PhraseEditorEditAsReactScript(Basis *_basis, QWidget *parent)
    : QWidget(parent), basis(_basis) {
  QLabel choose_path, specify_func_name, specify_amount_of_history, specify_memory_cells;
  LineEdit func_name;
  QSpinBox history_amount;
  QCheckBox needs_user_input;
  Spoiler other_props_widget;
  QGridLayout other_props_widget_layout, editor_layout;
  EditList *memory_cells = nullptr;
  choose_path.setText(tr("Specify script path:"));
  specify_func_name.setText(tr("Specify function name:"));
  auto *specify_history_amount = new QLabel(tr("Specify amount of message history to be sent:"), this);
  specify_history_amount.setWordWrap(true);
  needs_user_input.setText(tr("Check if script needs whole user input"));
  specify_memory_cells.setText(tr("Specify memory cells values to be sent:"));
  func_name.setPlaceholderText(tr("Function name..."));
  memory_cells = new EditList(this);

  editor_layout.addWidget(&choose_path, 0, 0);
  editor_layout.addWidget(&path, 0, 1);
  editor_layout.addWidget(&specify_func_name, 1, 0);
  editor_layout.addWidget(&func_name, 1, 1);
  editor_layout.addWidget(&specify_history_amount, 2, 0);
  editor_layout.addWidget(&history_amount, 2, 1);
  editor_layout.addWidget(&needs_user_input, 3, 0, 1, 2);
  editor_layout.addWidget(&other_props_widget, 4, 0, 1, 2);
  editor_layout.addWidget(&save_script_btn, 5, 0, 1, 2);
  setLayout(&editor_layout);
}

/*! @brief Loads script information from JSON. */
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
  memory_cells->append(script->memory_cells);
}

/*! @brief Clears the widget. */
void PhraseEditorEditAsReactScript::clear() {
  path.setText(tr("Select a file..."));
  func_name.setText("");
  history_amount.setValue(0);
  needs_user_input.setChecked(false);
  memory_cells->clear();
}

/*! @brief Saves the script. */
void PhraseEditorEditAsReactScript::save_script() {
  if (not script) script = new ReactScript();
  if (path.text() != tr("Select a file...")) script->path = path.text();
  else script->path = "";
  script->stype = ScriptType::React;
  script->fn_name = func_name.text();
  script->number_of_hist_messages = history_amount.value();
  script->needs_user_input = needs_user_input.isChecked();
  script->memory_cells = memory_cells->get_list();
  clear();
  emit save(ScriptsCast::to_string(script));
}

