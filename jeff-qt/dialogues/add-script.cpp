#include "add-script.h"

/*! @brief The constructor. */
AddScriptDialog::AddScriptDialog(QWidget *parent, Basis *_basis, 
                                 ExtensionsManager *_em, ModalHandler *m_handler) 
: ScrollFreezerWidget(parent), _m_handler(m_handler) {
  _m_handler->setPrisoner(this);
  setObjectName(object_name);
  auto *script_editor = new ScriptEditor(this, _basis, _em, _m_handler);
  auto *lt = new QGridLayout();
  lt->setMargin(0);
  lt->addWidget(script_editor, 0, 0);
  setLayout(lt);
  connect(script_editor, &ScriptEditor::closed, this, [this] { _m_handler->closePrisoner(); });
}
