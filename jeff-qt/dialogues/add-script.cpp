#include "add-script.h"

/*! @brief The constructor. */
AddScriptDialog::AddScriptDialog(QWidget *parent, Basis *_basis, 
                                 ExtensionsManager *_em, ModalHandler *m_handler) 
: ScrollFreezerWidget(parent), _m_handler(m_handler) {
  _m_handler->setPrisoner(this);
  auto *script_editor = new ScriptEditor(this, _basis, _em, _m_handler);
  setLayout(GridLt::another()->addw(script_editor));
  connect(script_editor, &ScriptEditor::closed, this, [this] { _m_handler->closePrisoner(); });
}
