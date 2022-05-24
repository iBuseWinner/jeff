#include "brief.h"

/*! @brief The constructor. */
PhraseEditorBrief::PhraseEditorBrief(Basis *_basis, QWidget *parent) 
  : QScrollArea(parent), basis(_basis) {
  // Top-level widgets.
  auto header_font = QApplication::font();
  header_font.setPointSize(14);
  header_font.setBold(true);
  header.setFont(header_font);
  header.setWordWrap(true);
  edit_expression.setText(tr("Edit text"));
  edit_expression.setIcon(QIcon::fromTheme("edit", QIcon(":/arts/icons/16/document-edit.svg")));
  connect(&edit_expression, &Button::clicked, this, &PhraseEditorBrief::edit_phrase_text);
  exec_checkbox.setText(tr("Process via script"));
  connect(&exec_checkbox, &QCheckBox::toggled, this, [this]() {
    if (exec_checkbox.isChecked()) edit_expression.setText(tr("Edit script"));
    else edit_expression.setText(tr("Edit text"));
  });
  back_to_overview.setText(tr("Back"));
  back_to_overview.setIcon(QIcon::fromTheme("go-previous", QIcon(":/arts/icons/16/go-previous.svg")));
  activators_list.setHeaderLabels({tr("Addresses"), tr("Activators")});
  activators_list.setWordWrap(true);
  activators_list.setContextMenuPolicy(Qt::CustomContextMenu);
  connect(&activators_list, &List::customContextMenuRequested,
          this, &PhraseEditorBrief::context_menu_for_activators);
  connect(&activators_list, &List::itemDoubleClicked, this, [this](QTreeWidgetItem *item) {
    emit open_brief(item);
  });
  reagents_list.setHeaderLabels({tr("Addresses"), tr("Reagents")});
  reagents_list.setWordWrap(true);
  reagents_list.setContextMenuPolicy(Qt::CustomContextMenu);
  connect(&reagents_list, &List::customContextMenuRequested,
          this, &PhraseEditorBrief::context_menu_for_reagents);
  connect(&reagents_list, &List::itemDoubleClicked, this, [this](QTreeWidgetItem *item) {
    emit open_brief(item);
  });
  auto *EAR_layout = new QHBoxLayout();
  EAR_layout->setSpacing(0);
  EAR_layout->setMargin(0);
  EAR_layout->addWidget(&activators_list);
  EAR_layout->addWidget(&reagents_list);
  auto *EAR_widget = new QWidget(this);
  EAR_widget->setLayout(EAR_layout);
  verticalScrollBar()->setStyleSheet(
    styling.css_scroll_bar.arg(styling.light_theme ? styling.css_light_sb : styling.css_dark_sb)
  );
  verticalScrollBar()->setFixedWidth(5);
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setFocusPolicy(Qt::NoFocus);
  setFrameStyle(QFrame::NoFrame);
  setFrameShadow(QFrame::Plain);
  setWidgetResizable(true);
  // Expression edit widgets.
  phrase_expression_edit_save.setIcon(
    QIcon::fromTheme("dialog-ok-apply", QIcon(":/arts/icons/16/dialog-ok-apply.svg")));
  connect(&phrase_expression_edit_save, &Button::clicked, this, &PhraseEditorBrief::save_phrase_text);
  phrase_expression_edit_layout.addWidget(&phrase_expression_edit_line);
  phrase_expression_edit_layout.addWidget(&phrase_expression_edit_save);
  phrase_expression_edit_widget.setLayout(&phrase_expression_edit_layout);
  phrase_expression_edit_widget.hide();
  // Context menus setup.
  add_phrase_action.setText(tr("New phrase"));
  add_phrase_action.setIcon(QIcon::fromTheme("list-add", QIcon(":/arts/icons/16/list-add.svg")));
  connect_phrase_action.setText(tr("Connect another phrase"));
  connect_phrase_action.setIcon(QIcon::fromTheme("insert-link", QIcon(":/arts/icons/16/insert-link.svg")));
  disconnect_phrase_action.setText(tr("Disconnect phrase"));
  disconnect_phrase_action.setIcon(
    QIcon::fromTheme("remove-link", QIcon(":/arts/icons/16/remove-link.svg")));
  phrases_context_menu.addAction(&add_phrase_action);
  phrases_context_menu.addAction(&connect_phrase_action);
  edit_phrase_action.setText(tr("Edit this phrase..."));
  edit_phrase_action.setIcon(QIcon::fromTheme("edit", QIcon(":/arts/icons/16/document-edit.svg")));
  remove_phrase_action.setText(tr("Delete this phrase"));
  remove_phrase_action.setIcon(QIcon::fromTheme("list-remove", QIcon(":/arts/icons/16/list-remove.svg")));
  // Layout setup.
  widget_layout.setSpacing(0);
  widget_layout.setMargin(0);
  widget_layout.addWidget(&header);
  widget_layout.addWidget(&edit_expression);
  widget_layout.addWidget(&address_label);
  widget_layout.addWidget(&exec_checkbox);
  widget_layout.addWidget(EAR_widget);
  widget_layout.addItem(new QSpacerItem(0, 0, QSizePolicy::Fixed, QSizePolicy::Expanding));
  widget_layout.addWidget(&back_to_overview);
  area_widget = new QWidget();
  area_widget->setLayout(&widget_layout);
  // Brief setup.
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setFocusPolicy(Qt::NoFocus);
  setFrameStyle(QFrame::NoFrame);
  setFrameShadow(QFrame::Plain);
  setWidgetResizable(true);
  setWidget(area_widget);
}

/*! @brief Limits the maximum size of the header so that long text does not stretch the editor. */
void PhraseEditorBrief::resizeEvent(QResizeEvent *event) { header.setMaximumWidth(width() - 10); }

/*! @brief Sets up brief widget. */
bool PhraseEditorBrief::setup(Source _source, Phrases _phrases, int address) {
  source = _source;
  phrases = _phrases;
  bool found = false;
  for (auto _phrase : phrases) {
    if (_phrase.address == address) {
      phrase = _phrase;
      found = true;
    }
  }
  if (not found) return false;
  QTreeWidgetItem *act_parent = activators_list.invisibleRootItem();
  QTreeWidgetItem *rea_parent = reagents_list.invisibleRootItem();
  activators_list.clear();
  reagents_list.clear();
  for (auto _phrase : phrases) {
    if (_phrase.links.contains(phrase.address))
      activators_list.addTopLevelItem(
        new QTreeWidgetItem(act_parent, {QString::number(_phrase.address), _phrase.expression})
      );
    if (phrase.links.contains(_phrase.address))
      reagents_list.addTopLevelItem(
        new QTreeWidgetItem(rea_parent, {QString::number(_phrase.address), _phrase.expression})
      );
  }
  header.setText(phrase.expression);
  address_label.setText(tr("The address of the phrase is %1.").arg(phrase.address));
  disconnect(&exec_checkbox, &QCheckBox::stateChanged, nullptr, nullptr);
  exec_checkbox.setChecked(phrase.exec == true ? Qt::Checked : Qt::Unchecked);
  connect(&exec_checkbox, &QCheckBox::stateChanged, this, &PhraseEditorBrief::change_exec);
  return true;
}

/*! @brief Edits the text of a phrase. */
void PhraseEditorBrief::edit_phrase_text() {
  edit_expression.setEnabled(false);
  back_to_overview.setEnabled(false);
  exec_checkbox.setEnabled(false);
  edit_expression.hide();
  header.hide();
  if (exec_checkbox.isChecked()) {
    if (not script_editor) {
      script_editor = new AddScriptDialog(nullptr, basis);
      script_editor->setMaximumWidth(maximumWidth() - 6);
    }
    auto json_script = header.text();
    script_editor->load_from_text(json_script);
    connect(
      script_editor, &AddScriptDialog::saved, this, &PhraseEditorBrief::save_script
    );
    connect(
      script_editor, &AddScriptDialog::closed, this, [this, json_script] { save_script(json_script); }
    );
    widget_layout.replaceWidget(&header, script_editor);
    script_editor->show();
  } else {
    phrase_expression_edit_line.setText(header.text());
    widget_layout.replaceWidget(&header, &phrase_expression_edit_widget);
    phrase_expression_edit_widget.show();
  }
}

/*! @brief Saves new text of a phrase. */
void PhraseEditorBrief::save_phrase_text() {
  auto text = phrase_expression_edit_line.text();
  if (text.isEmpty()) return;
  phrase.expression = text;
  if (not basis->sql->update_expression(source, phrase.expression, phrase.address)) return;
  phrase_expression_edit_widget.hide();
  header.setText(phrase.expression);
  widget_layout.replaceWidget(&phrase_expression_edit_widget, &header);
  header.show();
  edit_expression.show();
  exec_checkbox.setEnabled(true);
  edit_expression.setEnabled(true);
  back_to_overview.setEnabled(true);
}

/*! @brief Saves information about the script as JSON. */
void PhraseEditorBrief::save_script(QString script_json) {
  phrase.expression = script_json;
  if (not basis->sql->update_expression(source, phrase.expression, phrase.address)) return;
  script_editor->hide();
  header.setText(script_json);
  widget_layout.replaceWidget(script_editor, &header);
  disconnect(script_editor, &AddScriptDialog::saved, nullptr, nullptr);
  script_editor->setParent(nullptr);
  header.show();
  edit_expression.show();
  exec_checkbox.setEnabled(true);
  edit_expression.setEnabled(true);
  back_to_overview.setEnabled(true);
}

/*! @brief Changes the state of the exec field, saving the changes to the database. */
void PhraseEditorBrief::change_exec(int state) {
  exec_checkbox.setEnabled(false);
  bool exec = (state == Qt::Checked ? true : false);
  if (not basis->sql->update_exec(source, exec, phrase.address)) {
    exec_checkbox.setEnabled(true);
    disconnect(&exec_checkbox, &QCheckBox::stateChanged, nullptr, nullptr);
    exec_checkbox.setCheckState(state == Qt::Checked ? Qt::Unchecked : Qt::Checked);
    connect(&exec_checkbox, &QCheckBox::stateChanged, this, &PhraseEditorBrief::change_exec);
    return;
  }
  phrase.exec = exec;
  exec_checkbox.setEnabled(true);
}

/*! @brief Shows the context menu for the list of activators. */
void PhraseEditorBrief::context_menu_for_activators(const QPoint &pos) {
  auto *selected_item = activators_list.itemAt(pos);
  bool added = false;
  add_phrase_action.disconnect();
  connect_phrase_action.disconnect();
  connect(&add_phrase_action, &QAction::triggered, this, [this] {
    waits_for = PhraseEditorBriefWaitsFor::Activator;
    emit add_phrase_and_send_id();
  });
  connect(&connect_phrase_action, &QAction::triggered, this, [this] {
    waits_for = PhraseEditorBriefWaitsFor::Activator;
    emit open_selector();
  });
  if (selected_item) {
    added = true;
    phrases_context_menu.addAction(&edit_phrase_action);
    phrases_context_menu.addAction(&remove_phrase_action);
    phrases_context_menu.addAction(&disconnect_phrase_action);
    edit_phrase_action.disconnect();
    remove_phrase_action.disconnect();
    disconnect_phrase_action.disconnect();
    connect(&edit_phrase_action, &QAction::triggered, this, [this, selected_item] {
      emit open_brief(selected_item);
    });
    connect(&remove_phrase_action, &QAction::triggered, this, [this, selected_item] {
      auto address = selected_item->text(0).toInt();
      if (not basis->sql->remove_phrase(source, address)) return;
      for (int i = 0; i < phrases.length(); i++) {
        if (phrases[i].links.contains(address)) {
          phrases[i].links.remove(address);
          basis->sql->update_links(source, phrases[i].links, phrases[i].address);
        }
      }
      auto *item = activators_list.takeTopLevelItem(activators_list.indexOfTopLevelItem(selected_item));
      if (item) delete item;
      emit update_phrases();
    });
    connect(&disconnect_phrase_action, &QAction::triggered, this, [this, selected_item] {
      auto address = selected_item->text(0).toInt();
      for (int i = 0; i < phrases.length(); i++) {
        if (phrases[i].links.contains(address)) {
          phrases[i].links.remove(address);
          basis->sql->update_links(source, phrases[i].links, phrases[i].address);
        }
      }
      auto *item = activators_list.takeTopLevelItem(activators_list.indexOfTopLevelItem(selected_item));
      if (item) delete item;
      emit update_phrases();
    });
  }
  phrases_context_menu.exec(QCursor::pos());
  if (added) {
    phrases_context_menu.removeAction(&edit_phrase_action);
    phrases_context_menu.removeAction(&remove_phrase_action);
    phrases_context_menu.removeAction(&disconnect_phrase_action);
  }
}

/*! @brief Shows the context menu for the list of reagents. */
void PhraseEditorBrief::context_menu_for_reagents(const QPoint &pos) {
  auto *selected_item = reagents_list.itemAt(pos);
  bool added = false;
  add_phrase_action.disconnect();
  connect_phrase_action.disconnect();
  connect(&add_phrase_action, &QAction::triggered, this, [this] {
    waits_for = PhraseEditorBriefWaitsFor::Reagent;
    emit add_phrase_and_send_id();
  });
  connect(&connect_phrase_action, &QAction::triggered, this, [this] {
    waits_for = PhraseEditorBriefWaitsFor::Reagent;
    emit open_selector();
  });
  if (selected_item) {
    added = true;
    phrases_context_menu.addAction(&edit_phrase_action);
    phrases_context_menu.addAction(&remove_phrase_action);
    phrases_context_menu.addAction(&disconnect_phrase_action);
    edit_phrase_action.disconnect();
    remove_phrase_action.disconnect();
    disconnect_phrase_action.disconnect();
    connect(&edit_phrase_action, &QAction::triggered, this, [this, selected_item] {
      emit open_brief(selected_item);
    });
    connect(&remove_phrase_action, &QAction::triggered, this, [this, selected_item] {
      auto address = selected_item->text(0).toInt();
      if (not basis->sql->remove_phrase(source, address)) return;
      for (int i = 0; i < phrases.length(); i++) {
        if (phrases[i].links.contains(address)) {
          phrases[i].links.remove(address);
          basis->sql->update_links(source, phrases[i].links, phrases[i].address);
        }
      }
      auto *item = reagents_list.takeTopLevelItem(reagents_list.indexOfTopLevelItem(selected_item));
      if (item) delete item;
      emit update_phrases();
    });
    connect(&disconnect_phrase_action, &QAction::triggered, this, [this, selected_item] {
      auto address = selected_item->text(0).toInt();
      for (int i = 0; i < phrases.length(); i++) {
        if (phrases[i].links.contains(address)) {
          phrases[i].links.remove(address);
          basis->sql->update_links(source, phrases[i].links, phrases[i].address);
        }
      }
      auto *item = reagents_list.takeTopLevelItem(reagents_list.indexOfTopLevelItem(selected_item));
      if (item) delete item;
      emit update_phrases();
    });
  }
  phrases_context_menu.exec(QCursor::pos());
  if (added) {
    phrases_context_menu.removeAction(&edit_phrase_action);
    phrases_context_menu.removeAction(&remove_phrase_action);
    phrases_context_menu.removeAction(&disconnect_phrase_action);
  }
}

/*! @brief Adds a phrase to the list of activators or reagents and saves the changes to the database. */
void PhraseEditorBrief::waits_for_choosed(int address) {
  if (waits_for == PhraseEditorBriefWaitsFor::Nothing) return;
  List *widget = nullptr;
  if (waits_for == PhraseEditorBriefWaitsFor::Activator) {
    for (auto _phrase : phrases)
      if (_phrase.address == address) {
        _phrase.links.insert(phrase.address);
        if (basis->sql->update_links(source, _phrase.links, _phrase.address)) {
          widget = &activators_list;
          emit update_phrases();
        }
      }
  }
  else if (waits_for == PhraseEditorBriefWaitsFor::Reagent) {
    phrase.links.insert(address);
    if (basis->sql->update_links(source, phrase.links, phrase.address)) {
      emit update_phrases();
      widget = &reagents_list;
    }
  }
  waits_for = PhraseEditorBriefWaitsFor::Nothing;
  if (not widget) return;
  QTreeWidgetItem *list_parent = widget->invisibleRootItem();
  for (auto _phrase : phrases) {
    if (_phrase.address == address)
      widget->addTopLevelItem(
        new QTreeWidgetItem(list_parent, {QString::number(_phrase.address), _phrase.expression})
      );
  }
}
