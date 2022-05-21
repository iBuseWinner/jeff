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
  auto *path_info = new QLabel(tr("Specify script path:"), this);
  auto *stype_info = new QLabel(tr("Specify script type:"), this);
  path_input = new Button(tr("Select a file..."), this);
  QStringList stypes = {
    tr("Startup script (suitable for startup prompts)"),
    tr("Daemon (works in background with Jeff's server)"),
    tr("Server (works in background and receives all messages)"),
    tr("Custom scanner (another answering system)"),
    tr("Custom composer (receives chosed variants and answers on them in another manner)")
  };
  stype_input = new ComboBox(this);
  connect(
    stype_input, QOverload<int>::of(&QComboBox::currentIndexChanged),
    this, &AddScriptDialog::change_stype
  );
  stype_input->addItems(stypes);
  auto *cancel_btn = new Button(tr("Cancel"), this);
  auto *save_btn = new Button(tr("Add script"), this);
  if (not _m_handler) save_btn->setText(tr("Save script"));
  auto *buttons_layout = new QGridLayout();
  buttons_layout->setSpacing(0);
  buttons_layout->setMargin(0);
  buttons_layout->addWidget(cancel_btn, 0, 0);
  buttons_layout->addWidget(save_btn, 0, 1);
  auto *buttons_widget = new QWidget(this);
  buttons_widget->setContentsMargins(0, 0, 0, 0);
  buttons_widget->setLayout(buttons_layout);
  dynamic_properties_layout = new QGridLayout();
  dynamic_properties_layout->setSpacing(0);
  dynamic_properties_layout->setMargin(0);
  auto *dynamic_properties_widget = new QWidget(this);
  dynamic_properties_widget->setContentsMargins(0, 0, 0, 0);
  dynamic_properties_widget->setLayout(dynamic_properties_layout);
  auto *main_layout = new QGridLayout();
  main_layout->addWidget(path_info, 0, 0);
  main_layout->addWidget(path_input, 0, 1);
  main_layout->addWidget(stype_info, 1, 0);
  main_layout->addWidget(stype_input, 1, 1);
  main_layout->addWidget(dynamic_properties_widget, 2, 0, 2, 1);
  main_layout->addWidget(buttons_widget, 3, 0, 1, 2);
  setLayout(main_layout);
  if (_m_handler) {
    connect(save_btn, &Button::clicked, this, [this]() {
      // pm->add_script()
      _m_handler->closePrisoner();
    });
    connect(cancel_btn, &Button::clicked, this, [this]() { _m_handler->closePrisoner(); });
  }
}

/*! @brief TBD */
void AddScriptDialog::change_stype() {
  stype_input->setEnabled(false);
  auto index = stype_input->currentIndex();
  if (index == 0) {
    // Shows: 1) fn_name_info + fn_name_input
    //        2) memory_cells_info, memory_cells_list, add_memory_cell_btn, remove_memory_cell_btn
  } else if (index == 1) {
    
  } else if (index == 2) {
    
  } else if (index == 3) {
    
  } else if (index == 4) {
    
  }
  stype_input->setEnabled(true);
}

/*! @brief TBD */
ScriptMetadata *AddScriptDialog::get_script_from_metadata() { return nullptr; }
