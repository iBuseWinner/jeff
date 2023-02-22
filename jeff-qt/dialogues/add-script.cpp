#include "add-script.hpp"

/*! @brief The constructor. */
AddScriptDialog::AddScriptDialog(QWidget *parent, ModalHandler *mhandler, Basis *_basis) : Dialog(mhandler, parent) {
  script_editor = new ReactScriptEditor(this, _basis);
  setLayout(GridLt::another()->addw(script_editor));
  connect(script_editor, &ReactScriptEditor::closed, this, &Dialog::close);
}
