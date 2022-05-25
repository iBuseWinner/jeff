#include "add-script.h"

/*! @brief The constructor. */
AddScriptDialog::AddScriptDialog(QWidget *parent, Basis *_basis, 
                                 PythonModule *_pm, ModalHandler *m_handler) 
: ScrollFreezerWidget(parent), _m_handler(m_handler) {
  _m_handler->setPrisoner(this);
  setObjectName("add_script_dialog");
  auto *script_editor = new ScriptEditor(this, _basis, _pm, _m_handler);
  auto *lt = new QGridLayout();
  lt->setMargin(0);
  lt->addWidget(script_editor, 0, 0);
  setLayout(lt);
  connect(script_editor, &ScriptEditor::closed, this, [this] { _m_handler->closePrisoner(); });
}
