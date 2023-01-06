#include "scripteditor.h"

/*! TODO исправить все поля, что изменились
 *  TODO исправить сохранение CustomScan/CustomCompose  */

/*! @brief The constructor. */
ScriptEditor::ScriptEditor(QWidget *parent, Basis *_basis, SEModule *_sem, ModalHandler *m_handler) 
  : QWidget(parent), basis(_basis), sem(_sem), _m_handler(m_handler)
{
  stype_info = new QLabel(tr("Specify script type:"), this);
  auto *path_info = new QLabel(tr("Specify script path:"), this);
  path_input = new Button(tr("Select a file..."), this);
  connect(path_input, &Button::clicked, this, [this]() {
    auto _path = 
      QFileDialog::getOpenFileName(nullptr, tr("Select file..."), "", tr("Python script") + "(*.py);;" + tr("Any executable") + "(*.*)");
    if (not _path.isEmpty()) path_input->setText(_path);
    else path_input->setText(tr("Select a file..."));
  });
  stype_input = new ComboBox(this);
  stype_input->addItems({
    tr("Daemon (works in background with Jeff's server)"),
    tr("Server (works in background and receives all messages)"),
    tr("Custom scanner (another answering system)"),
    tr("Custom composer (receives chosed variants and answers on them in another manner)")
  });
  connect(
    stype_input, QOverload<int>::of(&QComboBox::currentIndexChanged),
    this, [this](int _stype) { change_stype(_stype); }
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
  if (stype == 0) {
    // Daemon (works in background with Jeff's server)
    dynamic_properties_layout->parentWidget()->hide();
    connect(save_btn, &Button::clicked, this, [this] {
      if (path_input->text() == tr("Select a file...")) {
        basis->warn_about(tr("Please complete path field before saving."));
        return;
      }
      auto *daemon_script = new DaemonScript();
      daemon_script->cmd = path_input->text();
      if (_m_handler) {
        sem->add_daemon(daemon_script);
        emit closed();
      } else {
        emit saved(ScriptsCast::to_string(daemon_script));
        delete daemon_script;
      }
    });
  } else if (stype == 1) {
    // Server (works in background and receives all messages)
    auto *server_addr_info = new QLabel(tr("If the script is located remotely, enter the IP address:"));
    server_addr_input = new LineEdit();
    server_addr_input->setPlaceholderText(tr("IP address..."));
    auto *ip_validator =
      new QRegularExpressionValidator(QRegularExpression(ipv4_range), server_addr_input);
    server_addr_input->setValidator(ip_validator);
    auto *server_port_info = new QLabel(tr("Enter server port:"));
    server_port_input = new LineEdit();
    server_port_input->setPlaceholderText(tr("Server port..."));
    auto *port_validator = new QIntValidator(1, 65535, server_port_input);
    server_port_input->setValidator(port_validator);
    dynamic_properties_layout->addWidget(server_addr_info, 0, 0);
    dynamic_properties_layout->addWidget(server_addr_input, 0, 1);
    dynamic_properties_layout->addWidget(server_port_info, 1, 0);
    dynamic_properties_layout->addWidget(server_port_input, 1, 1);
    connect(save_btn, &Button::clicked, this, [this] {
      if ((path_input->text() == tr("Select a file...") or server_addr_input->text().isEmpty()) and server_port_input->text().isEmpty()) {
        basis->warn_about(tr("Please complete path or server IP and port fields before saving."));
        return;
      }
      auto *server_script = new ServerScript();
      if (path_input->text() != tr("Select a file...")) server_script->cmd = path_input->text();
      auto addr = server_addr_input->text();
      int pos = 0;
      if (server_addr_input->validator()->validate(addr, pos) == QValidator::Acceptable)
        server_script->server_addr = QHostAddress(addr);
      auto port = server_port_input->text();
      if (server_port_input->validator()->validate(port, pos) == QValidator::Acceptable)
        server_script->server_port = port.toInt();
      if (_m_handler) {
        sem->add_daemon(server_script);
        emit closed();
      } else {
        emit saved(ScriptsCast::to_string(server_script));
        delete server_script;
      }
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
      auto *cs_script = new CustomScanScript();
      cs_script->path = path_input->text();
      cs_script->fn_name = fn_name_input->text();
      if (_m_handler) {
        // sem->add_script(cs_script);
        emit closed();
      } else {
        emit saved(ScriptsCast::to_string(cs_script));
        delete cs_script;
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
      auto *cc_script = new CustomComposeScript();
      cc_script->path = path_input->text();
      cc_script->fn_name = fn_name_input->text();
      cc_script->send_adprops = send_adprops->isChecked();
      if (_m_handler) {
        // sem->add_script(cc_script);
        emit closed();
      } else {
        emit saved(ScriptsCast::to_string(cc_script));
        delete cc_script;
      }
    });
  } else if (stype == 4) {
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
      auto *react_script = new ReactScript();
      react_script->path = path_input->text();
      react_script->memory_cells = memory_cells_list->get_list();
      react_script->fn_name = fn_name_input->text();
      react_script->hist_parts = hist_amount_input->value();
      react_script->needs_user_input = needs_ue_input->isChecked();
      emit saved(ScriptsCast::to_string(react_script));
      delete react_script;
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
  auto *script = ScriptsCast::to_script(json_text);
  if (not script) return false;
  return load_from_script(script);
}

/*! @brief Loads data into a form from a saved state. */
bool ScriptEditor::load_from_script(ScriptMetadata *script) {
  if (script->stype == ScriptType::Daemon) {
    change_stype(0);
    auto *s = dynamic_cast<DaemonScript *>(script);
    if (not s) return false;
    path_input->setText(s->cmd);
    return true;
  } else if (script->stype == ScriptType::Server) {
    change_stype(1);
    auto *s = dynamic_cast<ServerScript *>(script);
    if (not s) return false;
    path_input->setText(s->cmd);
    server_addr_input->setText(s->server_addr.toString());
    server_port_input->setText(QString::number(s->server_port));
    return true;
  } else if (script->stype == ScriptType::CustomScan) {
    change_stype(2);
    auto *s = dynamic_cast<CustomScanScript *>(script);
    if (not s) return false;
    path_input->setText(s->path);
    fn_name_input->setText(s->fn_name);
    return true;
  } else if (script->stype == ScriptType::CustomCompose) {
    change_stype(3);
    auto *s = dynamic_cast<CustomComposeScript *>(script);
    if (not s) return false;
    path_input->setText(s->path);
    fn_name_input->setText(s->fn_name);
    send_adprops->setChecked(s->send_adprops);
    return true;
  } else if (script->stype == ScriptType::React) {
    set_stype(4);
    auto *s = dynamic_cast<ReactScript *>(script);
    if (not s) return false;
    path_input->setText(s->path);
    fn_name_input->setText(s->fn_name);
    hist_amount_input->setValue(s->hist_parts);
    needs_ue_input->setChecked(s->needs_user_input);
    memory_cells_list->append(s->memory_cells);
    return true;
  } else return false;
}
