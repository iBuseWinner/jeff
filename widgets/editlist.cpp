#include "editlist.h"

/*! @brief The constructor. */
EditList::EditList(QWidget *parent) : QWidget(parent) {
  list_widget = new List(this);
  line_edit = new LineEdit(this);
  add_btn = new Button(QString(), this);
  remove_selected_btn = new Button(QString(), this);
  add_btn->setIcon(QIcon::fromTheme("list-add", QIcon(":/arts/icons/16/list-add.svg")));
  remove_selected_btn->setIcon(
    QIcon::fromTheme("list-remove", QIcon(":/arts/icons/16/list-remove.svg")));
  auto *buttons_widget_layout = new QVBoxLayout();
  buttons_widget_layout->setMargin(0);
  buttons_widget_layout->addWidget(add_btn);
  buttons_widget_layout->addWidget(remove_selected_btn);
  buttons_widget_layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Fixed, QSizePolicy::Expanding));
  auto *buttons_widget = new QWidget(this);
  buttons_widget->setContentsMargins(0, 0, 0, 0);
  buttons_widget->setLayout(buttons_widget_layout);
  auto *main_layout = new QGridLayout();
  main_layout->setMargin(0);
  main_layout->addWidget(line_edit, 0, 0, 1, 2);
  main_layout->addWidget(list_widget, 1, 0);
  main_layout->addWidget(buttons_widget, 1, 1);
  setLayout(main_layout);
  connect(add_btn, &Button::clicked, this, &EditList::add_element);
  connect(remove_selected_btn, &Button::clicked, this, &EditList::remove_element);
}

/*! @brief Returns a list of elements as a list of strings. */
QStringList EditList::get_list() {
  QStringList list;
  auto *root = list_widget->invisibleRootItem();
  for (int i = 0; i < root->childCount(); i++) list.append(root->child(i)->text(0));
  return list;
}

/*! @brief Adds the given elements to the list. */
void EditList::append(QStringList list) {
  auto *root = list_widget->invisibleRootItem();
  for (auto element : list) auto *list_widget_element = new QTreeWidgetItem(root, {element});
}

/*! @brief Set the text on the buttons. */
void EditList::set_add_btn_text(QString text) { add_btn->setText(text); }
void EditList::set_rem_btn_text(QString text) { remove_selected_btn->setText(text); }

/*! @brief Tells if the list is empty. */
bool EditList::is_empty() { return list_widget->topLevelItemCount(); }

/*! @brief Sets the placeholder text for the input line. */
void EditList::set_lineedit_placeholder_text(QString placeholder_text) {
  line_edit->setPlaceholderText(placeholder_text);
}

/*! @brief Sets headers to the list. */
void EditList::set_list_headers(QStringList headers) {
  list_widget->setHeaderLabels(headers);
}

/*! @brief Clears the list. */
void EditList::clear() {
  auto *root = list_widget->invisibleRootItem();
  while (root->childCount()) {
    auto *item = root->takeChild(0);
    delete item;
  }
  line_edit->setText(QString());
}

/*! @brief Adds new element with a text from the input line. */
void EditList::add_element() {
  auto element_text = line_edit->text();
  if (element_text.trimmed().isEmpty()) return;
  line_edit->setText(QString());
  auto *root = list_widget->invisibleRootItem();
  auto *list_widget_element = new QTreeWidgetItem(root, {element_text});
}

/*! @brief Removes selected element. */
void EditList::remove_element() {
  auto *root = list_widget->invisibleRootItem();
  auto selected = list_widget->selectedItems();
  for (auto *item : selected) { root->removeChild(item); delete item; }
}
