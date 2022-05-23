#include "editlist.h"

/*! @brief The constructor. */
EditListWidget::EditListWidget(QWidget *parent) : QWidget(parent) {
  list_widget = new List(this);
  line_edit = new LineEdit(this);
  add_btn = new Button("", this);
  remove_selected_btn = new Button("", this);
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
  connect(add_btn, &Button::clicked, this, &EditListWidget::add_element);
  connect(remove_selected_btn, &Button::clicked, this, &EditListWidget::remove_element);
}

/*! @brief TBD */
QStringList EditListWidget::get_list() {
  QStringList list;
  auto *root = list_widget->invisibleRootItem();
  for (int i = 0; i < root->childCount(); i++) list.append(root->child(i)->text(0));
  return list;
}

/*! @brief TBD */
void EditListWidget::set_list(QStringList list) {
  auto *root = list_widget->invisibleRootItem();
  for (auto element : list) auto *list_widget_element = new QTreeWidgetItem(root, {element});
}

/*! @brief TBD */
void EditListWidget::set_add_btn_text(QString text) { add_btn->setText(text); }

/*! @brief TBD */
void EditListWidget::set_rem_btn_text(QString text) { remove_selected_btn->setText(text); }

/*! @brief TBD */
void EditListWidget::set_lineedit_placeholder_text(QString placeholder_text) {
  line_edit->setPlaceholderText(placeholder_text);
}

/*! @brief TBD */
void EditListWidget::set_list_headers(QStringList headers) {
  list_widget->setHeaderLabels(headers);
}

/*! @brief TBD */
void EditListWidget::clear() {
  auto *root = list_widget->invisibleRootItem();
  while (root->childCount()) {
    auto *item = root->takeChild(0);
    delete item;
  }
  line_edit->setText(QString());
}

/*! @brief TBD */
void EditListWidget::add_element() {
  auto element_text = line_edit->text();
  if (element_text.trimmed().isEmpty()) return;
  line_edit->setText(QString());
  auto *root = list_widget->invisibleRootItem();
  auto *list_widget_element = new QTreeWidgetItem(root, {element_text});
}

/*! @brief TBD */
void EditListWidget::remove_element() {
  auto *root = list_widget->invisibleRootItem();
  root->takeChildren();
}
