#include "add-script.hpp"

/*! @brief The constructor. */
AddScriptDialog::AddScriptDialog(QWidget *parent, ModalHandler *mhandler, 
                                 Basis *_basis, ExtensionsManager *_em) 
: Dialog(mhandler, parent) {
  auto *script_editor = new ScriptEditor(this, _basis, _em, mhandler);
  setLayout(GridLt::another()->addw(script_editor));
  connect(script_editor, &ScriptEditor::closed, this, &Dialog::close);
}
