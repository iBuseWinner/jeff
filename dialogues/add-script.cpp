#include "add-script.h"

/*! @brief The constructor. */
AddScriptDialog::AddScriptDialog(
  QWidget *parent = nullptr, Basis *_basis = nullptr, PythonModule *_pm = nullptr, 
  ModalHandler *m_handler = nullptr
) : ScrollFreezerWidget(parent), basis(_basis), pm(_pm), _m_handler(m_handler) {
  if (_m_handler) {
    _m_handler->setPrisoner(this);
    setObjectName("add_script_dialog");
  }
  auto *path_info = new QLabel(tr("Specify script path:"), this);
  auto *stype_info = new QLabel(tr("Specify script type:"), this);
  path_input = new Button(tr("Select a file..."), this);
  stype_input = new ComboBox(this);
  stype_input->addItems({
    tr("Startup script (suitable for startup prompts)"),
    tr("Daemon (works in background with Jeff's server)"),
    tr("Server (works in background and receives all messages)"),
    tr("Custom scanner (another answering system)"),
    tr("Custom composer (receives chosed variants and answers on them in another manner)")
  });
  connect(
    stype_input, QOverload<int>::of(&QComboBox::currentIndexChanged),
    this, [this] { change_stype(); }
  );
  auto *cancel_btn = new Button(tr("Cancel"), this);
  cancel_btn->setIcon(QIcon::fromTheme("window-close", QIcon(":/arts/icons/16/window-close.svg")));
  save_btn = new Button(tr("Add script"), this);
  save_btn->setIcon(QIcon::fromTheme("dialog-ok-apply", QIcon(":/arts/icons/16/dialog-ok-apply.svg")));
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
  dynamic_properties_layout->setMargin(0);
  auto *dynamic_properties_widget = new QWidget(this);
  dynamic_properties_widget->setContentsMargins(0, 0, 0, 0);
  dynamic_properties_widget->setLayout(dynamic_properties_layout);
  auto *main_layout = new QGridLayout();
  main_layout->addWidget(path_info, 0, 0);
  main_layout->addWidget(path_input, 0, 1);
  main_layout->addWidget(stype_info, 1, 0);
  main_layout->addWidget(stype_input, 1, 1);
  main_layout->addWidget(dynamic_properties_widget, 2, 0, 1, 2);
  main_layout->addWidget(buttons_widget, 3, 0, 1, 2);
  setLayout(main_layout);
  if (_m_handler) connect(cancel_btn, &Button::clicked, this, [this] { _m_handler->closePrisoner(); });
  change_stype();
}

/*! @brief The destructor. */
AddScriptDialog::~AddScriptDialog() {
  QLayoutItem *child = nullptr;
  if (dynamic_properties_layout->count()) 
    while ((child = dynamic_properties_layout->takeAt(0)) != nullptr) {
      delete child->widget();
      delete child;
    }
}

/*! @brief TBD */
void AddScriptDialog::change_stype() {
  stype_input->setEnabled(false);
  QLayoutItem *child = nullptr;
  if (dynamic_properties_layout->count()) 
    while ((child = dynamic_properties_layout->takeAt(0)) != nullptr) {
      delete child->widget();
      delete child;
    }
  auto index = stype_input->currentIndex();
  if (index == 0) {
    auto *fn_name_info = new QLabel(tr("Specify function name:"));
    auto *fn_name_input = new LineEdit();
    fn_name_input->setPlaceholderText(tr("Function name..."));
    auto *memory_cells_info = new QLabel(tr("Add the memory cells to be passed to the script:"));
    auto *memory_cells_list = new EditListWidget();
    memory_cells_list->set_add_btn_text(tr("Add memory cell"));
    memory_cells_list->set_rem_btn_text(tr("Remove selected cell"));
    memory_cells_list->set_lineedit_placeholder_text(tr("Memory cell name..."));
    memory_cells_list->set_list_headers({tr("Memory cell key")});
    dynamic_properties_layout->addWidget(fn_name_info, 0, 0);
    dynamic_properties_layout->addWidget(fn_name_input, 1, 0);
    dynamic_properties_layout->addWidget(memory_cells_info, 2, 0);
    dynamic_properties_layout->addWidget(memory_cells_list, 3, 0);
  } else if (index == 1) {
    
  } else if (index == 2) {
    
  } else if (index == 3) {
    
  } else if (index == 4) {
    
  }
  stype_input->setEnabled(true);
}

/*! @brief TBD */
ScriptMetadata *AddScriptDialog::get_script_from_metadata() { return nullptr; }
