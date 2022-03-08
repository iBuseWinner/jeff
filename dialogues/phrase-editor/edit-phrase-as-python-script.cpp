#include "edit-phrase-as-python-script.h"

/*! @brief The constructor. */
PhraseEditorEditAsPython::PhraseEditorEditAsPython(Basis *_basis, QWidget *parent)
    : QWidget(parent), basis(_basis) {
  choose_path.setText(tr("Choose script path:"));
  specify_func_name.setText(tr("Specify function name:"));
  specify_amount_of_history.setText(tr("Specify amount of message history to be sent:"));
  specify_memory_cells.setText(tr("Specify memory cells values to be sent:"));
  path.setText(tr("Select a file..."));
  connect(&path, &Button::clicked, this, &PhraseEditorEditAsPython::select_file);
  add_memory_cell.setText(tr("Add memory cell"));
  remove_memory_cell.setText(tr("Remove this cell"));
  save_script.setIcon(
    QIcon::fromTheme("dialog-ok-apply", QIcon(":/arts/icons/16/dialog-ok-apply.svg")));
  connect(&save_script, &Button::clicked, this, &PhraseEditorEditAsPython::save);
}

/*! @brief TBD */
void PhraseEditorEditAsPython::select_file() {
  _path = QFileDialog::getOpenFileName(nullptr, tr("Select file..."), "", tr("Python script") + "(*.py)");
  if (not _path.isEmpty()) {
    path.setText(_path);
  } else path.setText(tr("Select a file..."));
}
