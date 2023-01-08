#include "create-source.h"

/*! @brief The constructor. */
CreateSourceDialog::CreateSourceDialog(Basis *_basis, QWidget *parent) : QWidget(parent), basis(_basis) {
  title_edit.setPlaceholderText(tr("Enter source name"));
  connect(&select, &Button::clicked, this, &CreateSourceDialog::select_file);
  save.setText(tr("Save"));
  save.setIcon(QIcon::fromTheme("document-save", QIcon(":/arts/icons/16/document-save.svg")));
  connect(&save, &Button::clicked, this, &CreateSourceDialog::create_source);
  cancel.setText(tr("Cancel"));
  cancel.setIcon(QIcon::fromTheme("window-close", QIcon(":/arts/icons/16/window-close.svg")));
  connect(&cancel, &Button::clicked, this, &QWidget::close);
  layout.addWidget(&title_edit, 0, 0, 1, 0);
  layout.addWidget(&select, 1, 0, 1, 0);
  layout.addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Fixed), 2, 0);
  layout.addWidget(&save, 2, 1);
  layout.addWidget(&cancel, 2, 2);
  setLayout(&layout);
  fill_select_file_btn();
}

/*! @brief Turns the faceless button into a button for selecting a database. */
void CreateSourceDialog::fill_select_file_btn() {
  select.setText(tr("Select database file..."));
  select.setIcon(QIcon::fromTheme("document-open", QIcon(":/arts/icons/16/document-open.svg")));
}

/*! @brief Opens the database selection window and customizes the button. */
void CreateSourceDialog::select_file() {
  path = QFileDialog::getSaveFileName(
      nullptr, tr("Select database..."), "", tr("Jeff database") + "(*.j.db)",
      nullptr, QFileDialog::DontConfirmOverwrite);
  if (not path.isEmpty()) {
    select.setText(path);
    select.setIcon(QIcon::fromTheme("document-open", QIcon(":/arts/icons/16/db-file.svg")));
  } else fill_select_file_btn();
}

/*! @brief Finishes configuring the future source. */
void CreateSourceDialog::create_source() {
  if (path.isEmpty() or title_edit.text().isEmpty()) close();
  Source source;
  source.path = path;
  source.table_title = title_edit.text();
  basis->sql->create_source(source, &(source.table_name));
  emit add(source);
  close();
}
