#include "scripteditor.h"

/*! TODO исправить все поля, что изменились
 *  TODO исправить сохранение CustomScan/CustomCompose  */

/*! @brief The constructor. */
ScriptEditor::ScriptEditor(QWidget *parent, Basis *_basis, ExtensionsManager *_em, ModalHandler *mhandler) 
  : QWidget(parent), basis(_basis), em(_em), _mhandler(mhandler)
{
  stype_info = new QLabel(tr("Specify script type:"), this);
  auto *path_info = new QLabel(tr("Specify script path:"), this);
  path_input = new Button(tr("Select a file..."), this);
  connect(path_input, &Button::clicked, this, [this] {
    set_path(
      QFileDialog::getOpenFileName(nullptr, tr("Select file..."), "", tr("Python script") + "(*.py);;" + tr("Any executable") + "(*.*)")
    );
  });
  stype_input = new ComboBox(this);
  stype_input->addItems({
    tr("Custom scanner (another answering system)"),
    tr("Custom composer (receives chosed variants and answers on them in another manner)")
  });
  connect(
    stype_input, QOverload<int>::of(&QComboBox::currentIndexChanged),
    this, [this](int _stype) { change_stype(_stype + 2); }
  );
  auto *cancel_btn = new Button(tr("Cancel"), this);
  cancel_btn->setIcon(QIcon::fromTheme("window-close", QIcon(":/arts/icons/16/window-close.svg")));
  save_btn = new Button(tr("Add script"), this);
  save_btn->setIcon(QIcon::fromTheme("dialog-ok-apply", QIcon(":/arts/icons/16/dialog-ok-apply.svg")));
  if (not _mhandler) save_btn->setText(tr("Save script"));
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
  main_layout->addWidget(stype_info, 0, 0);
  main_layout->addWidget(stype_input, 0, 1);
  main_layout->addWidget(path_info, 1, 0);
  main_layout->addWidget(path_input, 1, 1);
  main_layout->addWidget(dynamic_properties_widget, 2, 0, 1, 2);
  main_layout->addWidget(buttons_widget, 3, 0, 1, 2);
  setLayout(main_layout);
  connect(cancel_btn, &Button::clicked, this, [this] { emit closed(); });
  change_stype();
}

/*! @brief The destructor. */
ScriptEditor::~ScriptEditor() {
  QLayoutItem *child = nullptr;
  if (dynamic_properties_layout->count())
    while ((child = dynamic_properties_layout->takeAt(0)) != nullptr) {
      delete child->widget();
      delete child;
    }
}

/*! @brief Shows a name of the file and sets correct icons. */
void ScriptEditor::set_path(QString path) {
  if (not path.isEmpty()) {
    filepath = path;
    QFileInfo fi(filepath);
    path_input->setText(fi.fileName());
    path_input->setIcon(QIcon::fromTheme("edit", QIcon(":/arts/icons/16/document-edit.svg")));
  } else {
    path_input->setText(tr("Select a file..."));
    path_input->setIcon(QIcon::fromTheme("insert-link", QIcon(":/arts/icons/16/insert-link.svg")));
    filepath.clear();
  }
}

/*! @brief Changes the layout depending on the selected script type. */
void ScriptEditor::change_stype() {
  disconnect(save_btn, &Button::clicked, nullptr, nullptr);
  if (dynamic_properties_layout->parentWidget()->isHidden())
    dynamic_properties_layout->parentWidget()->show();
  stype_input->setEnabled(false);
  QLayoutItem *child = nullptr;
  if (dynamic_properties_layout->count()) 
    while ((child = dynamic_properties_layout->takeAt(0)) != nullptr) {
      delete child->widget();
      delete child;
    }
  if (stype == 1) {
    // React script (runs when a pattern is found in user input)
    /*! @details This option is not listed and is responsible for editing react scripts saved as 
     *  phrase text in the database. */
    auto *fn_name_info = new QLabel(tr("Specify function name:"));
    fn_name_input = new LineEdit();
    fn_name_input->setPlaceholderText(tr("Function name..."));
    auto *hist_amount_info = new QLabel(tr("Specify amount of message history to be sent:"));
    hist_amount_info->setWordWrap(true);
    hist_amount_input = new QSpinBox();
    needs_ue_input = new QCheckBox(tr("Check if script needs whole user input"));
    auto *memory_cells_info = new QLabel(tr("Add the memory cells to be passed to the script:"));
    memory_cells_list = new EditList();
    memory_cells_list->set_add_btn_text(tr("Add memory cell"));
    memory_cells_list->set_rem_btn_text(tr("Remove selected cell"));
    memory_cells_list->set_lineedit_placeholder_text(tr("Memory cell name..."));
    memory_cells_list->set_list_headers({tr("Cells list")});
    dynamic_properties_layout->addWidget(fn_name_info, 0, 0);
    dynamic_properties_layout->addWidget(fn_name_input, 0, 1);
    dynamic_properties_layout->addWidget(hist_amount_info, 1, 0);
    dynamic_properties_layout->addWidget(hist_amount_input, 1, 1);
    dynamic_properties_layout->addWidget(needs_ue_input, 2, 0, 1, 2);
    dynamic_properties_layout->addWidget(memory_cells_info, 3, 0, 1, 2);
    dynamic_properties_layout->addWidget(memory_cells_list, 4, 0, 1, 2);
    connect(save_btn, &Button::clicked, this, [this] {
      if (
        path_input->text() == tr("Select a file...") or
        fn_name_input->text().isEmpty()
      ) {
        basis->warn_about(tr("Please complete path and function name fields before saving."));
        return;
      }
      auto *react = new ScriptMeta();
      react->filepath = filepath;
      react->required_memory_cells = memory_cells_list->get_list();
      react->fn_name = fn_name_input->text();
      react->required_history_parts = hist_amount_input->value();
      react->required_user_input = needs_ue_input->isChecked();
      emit saved(react->to_string());
      delete react;
    });
  } else if (stype == 2) {
    // Custom scanner (another answering system)
    auto *fn_name_info = new QLabel(tr("Specify function name:"));
    fn_name_input = new LineEdit();
    fn_name_input->setPlaceholderText(tr("Function name..."));
    dynamic_properties_layout->addWidget(fn_name_info, 0, 0);
    dynamic_properties_layout->addWidget(fn_name_input, 0, 1);
    connect(save_btn, &Button::clicked, this, [this] {
      if ((path_input->text() == tr("Select a file...") or fn_name_input->text().isEmpty())) {
        basis->warn_about(tr("Please complete path and function name fields before saving."));
        return;
      }
      auto *scanner = new ScriptMeta();
      scanner->filepath = filepath;
      scanner->fn_name = fn_name_input->text();
      if (_mhandler) {
        // sem->add_script(scanner);
        emit closed();
      } else {
        emit saved(scanner->to_string());
        delete scanner;
      }
    });
  } else if (stype == 3) {
    // Custom composer (receives chosed variants and answers on them in another manner)
    auto *fn_name_info = new QLabel(tr("Specify function name:"));
    fn_name_input = new LineEdit();
    fn_name_input->setPlaceholderText(tr("Function name..."));
    send_adprops = new QCheckBox(tr("Check if you need to pass additional values to the script"));
    dynamic_properties_layout->addWidget(fn_name_info, 0, 0);
    dynamic_properties_layout->addWidget(fn_name_input, 0, 1);
    dynamic_properties_layout->addWidget(send_adprops, 1, 0, 1, 2);
    connect(save_btn, &Button::clicked, this, [this] {
      if ((path_input->text() == tr("Select a file...") or fn_name_input->text().isEmpty())) {
        basis->warn_about(tr("Please complete path and function name fields before saving."));
        return;
      }
      auto *composer = new ScriptMeta();
      composer->filepath = filepath;
      composer->fn_name = fn_name_input->text();
      composer->required_adprops = send_adprops->isChecked();
      if (_mhandler) {
        // sem->add_script(composer);
        emit closed();
      } else {
        emit saved(composer->to_string());
        delete composer;
      }
    });
  }
  stype_input->setEnabled(true);
}

/*! @brief Changes the script type and then calls the layout change function. */
void ScriptEditor::change_stype(int _stype) { stype = _stype; change_stype(); }

/*! @brief Fixes the script type, making it immutable. */
void ScriptEditor::set_stype(int _stype) {
  disconnect(stype_input, QOverload<int>::of(&QComboBox::currentIndexChanged), nullptr, nullptr);
  stype_input->setEnabled(false);
  stype_input->hide();
  stype_info->hide();
  change_stype(_stype);
}

/*! @brief Loads data into the form from a string. */
bool ScriptEditor::load_from_text(QString json_text) {
  if (json_text.isEmpty()) return false;
  auto *script = ScriptMeta::from_string(json_text);
  if (not script) return false;
  return load_from_script(script);
  return false;
}

/*! @brief Loads data into a form from a saved state. */
bool ScriptEditor::load_from_script(ScriptMeta *script_meta) {
  if (script_meta->stype == ScriptType::React) {
    set_stype(1);
    set_path(script_meta->filepath);
    fn_name_input->setText(script_meta->fn_name);
    hist_amount_input->setValue(script_meta->required_history_parts);
    needs_ue_input->setChecked(script_meta->required_user_input);
    memory_cells_list->append(script_meta->required_memory_cells);
    return true;
  } else if (script_meta->stype == ScriptType::CustomScan) {
    change_stype(2);
    set_path(script_meta->filepath);
    fn_name_input->setText(script_meta->fn_name);
    return true;
  } else if (script_meta->stype == ScriptType::CustomCompose) {
    change_stype(3);
    set_path(script_meta->filepath);
    fn_name_input->setText(script_meta->fn_name);
    send_adprops->setChecked(script_meta->required_adprops);
    return true;
  } else return false;
}
