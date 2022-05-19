#include "add-script.h"

/*! @brief The constructor. */
AddScriptDialog::AddScriptDialog(
  QWidget *parent = nullptr, Basis *_basis = nullptr, PythonModule *_pm = nullptr, 
  ModalHandler *m_handler = nullptr
) : QWidget(parent), basis(_basis), pm(_pm), _m_handler(m_handler) {
  if (_m_handler) {
    _m_handler->setPrisoner(this);
    setObjectName("add_script_dialog");
  }
  path_info.setText(tr("Specify script path:"));
  path_input.setText(tr("Select a file..."));
  stype_info.setText(tr("Specify script type:"));
  QStringList stypes = {
    tr("Startup script (suitable for startup prompts)"),
    tr("Daemon (works in background with Jeff's server)"),
    tr("Server (works in background and receives all messages)"),
    tr("Custom scanner (another answering system)"),
    tr("Custom composer (receives chosed variants and answers on them in another manner)")
  };
  connect(
    &stype_input, QOverload<int>::of(&QComboBox::currentIndexChanged),
    this, &AddScriptDialog::change_stype
  );
  stype_input.addItems(stypes);
  cancel_btn.setText(tr("Cancel"));
  if (_m_handler) save_btn.setText(tr("Add script"));
  else save_btn.setText(tr("Save script"));
  fn_name_info.setText(tr("Specify function name:"));
  fn_name_info.setVisible(false);
  fn_name_input.setPlaceholderText(tr("Function name..."));
  fn_name_input.setVisible(false);
  memory_cells_info.setText(tr("Specify memory cells values to be sent:"));
  memory_cells_info.setWordWrap(true);
  memory_cells_info.setVisible(false);
  edit_list.set_list_headers({tr("Memory cell key")});
  edit_list.setVisible(false);
  buttons_layout.setSpacing(0);
  buttons_layout.setMargin(0);
  buttons_layout.addWidget(&cancel_btn, 0, 0);
  buttons_layout.addWidget(&save_btn, 0, 1);
  buttons_widget.setContentsMargins(0, 0, 0, 0);
  buttons_widget.setLayout(&buttons_layout);
  dynamic_properties_layout.setSpacing(0);
  dynamic_properties_layout.setMargin(0);
  dynamic_properties_widget.setContentsMargins(0, 0, 0, 0);
  dynamic_properties_widget.setLayout(&dynamic_properties_layout);
  main_layout.addWidget(&path_info, 0, 0);
  main_layout.addWidget(&path_input, 0, 1);
  main_layout.addWidget(&stype_info, 1, 0);
  main_layout.addWidget(&stype_input, 1, 1);
  main_layout.addWidget(&dynamic_properties_widget, 2, 0, 2, 1);
  main_layout.addWidget(&buttons_widget, 3, 0, 1, 2);
  setLayout(&main_layout);
  if (_m_handler) {
    connect(&save_btn, &Button::clicked, this, [this]() {
      // pm->add_script()
      _m_handler->closePrisoner();
    });
    connect(&cancel_btn, &Button::clicked, this, [this]() { _m_handler->closePrisoner(); });
  }
}

/*! @brief TBD */
void AddScriptDialog::change_stype() {
  stype_input.setEnabled(false);
  auto index = stype_input.currentIndex();
  if (index == 0) {
    // Shows: 1) fn_name_info + fn_name_input
    //        2) memory_cells_info, memory_cells_list, add_memory_cell_btn, remove_memory_cell_btn
  } else if (index == 1) {
    
  } else if (index == 2) {
    
  } else if (index == 3) {
    
  } else if (index == 4) {
    
  }
  stype_input.setEnabled(true);
}

/*! @brief TBD */
ScriptMetadata *AddScriptDialog::get_script_from_metadata() { return nullptr; }
