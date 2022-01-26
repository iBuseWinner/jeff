#include "create-source.h"

/*!
 * @fn CreateSourceDialog::CreateSourceDialog
 * @brief The constructor.
 * @param _basis reference to Basis instance
 * @param parent QObject parent
 */
CreateSourceDialog::CreateSourceDialog(Basis *_basis, QWidget *parent)
    : QWidget(parent), basis(_basis) {
  auto *grid_layout = new QGridLayout();
  titleInput = new LineEdit(this);
  selectFileBtn = new Button("", this);
  saveBtn = new Button(tr("Save"), this);
  cancelBtn = new Button(tr("Cancel"), this);
  auto *spacer = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Fixed);
  titleInput->setPlaceholderText(tr("Enter source name"));
  saveBtn->setIcon(
    QIcon::fromTheme("document-save", QIcon(":/arts/icons/16/document-save.svg")));
  cancelBtn->setIcon(
    QIcon::fromTheme("window-close", QIcon(":/arts/icons/16/window-close.svg")));
  grid_layout->addWidget(titleInput, 0, 0, 1, 0);
  grid_layout->addWidget(selectFileBtn, 1, 0, 1, 0);
  grid_layout->addItem(spacer, 2, 0);
  grid_layout->addWidget(saveBtn, 2, 1);
  grid_layout->addWidget(cancelBtn, 2, 2);
  setLayout(grid_layout);
  connector();
  selStart();
}

/*!
 * @fn CreateSourceDialog::connector
 * @brief Establishes communications for user interaction through the widget.
 */
void CreateSourceDialog::connector() {
  connect(selectFileBtn, &Button::clicked, this, &CreateSourceDialog::select);
  connect(saveBtn, &Button::clicked, this, &CreateSourceDialog::save);
  connect(cancelBtn, &Button::clicked, this, &QWidget::close);
}

/*!
 * @fn CreateSourceDialog::selStart
 * @brief Turns the faceless button into a button for selecting a database.
 */
void CreateSourceDialog::selStart() {
  selectFileBtn->setText(tr("Select database file..."));
  selectFileBtn->setIcon(
    QIcon::fromTheme("document-open", QIcon(":/arts/icons/16/document-open.svg")));
}

/*!
 * @fn CreateSourceDialog::select
 * @brief Opens the database selection window and customizes the button.
 */
void CreateSourceDialog::select() {
  m_dbpath = QFileDialog::getSaveFileName(
      nullptr, tr("Select database..."), "", tr("Jeff database") + "(*.j.db)",
      nullptr, QFileDialog::DontConfirmOverwrite);
  if (!m_dbpath.isEmpty()) {
    selectFileBtn->setText(m_dbpath);
    selectFileBtn->setIcon(
      QIcon::fromTheme("document-open", QIcon(":/arts/icons/16/db-file.svg")));
  } else selStart();
}

/*!
 * @fn CreateSourceDialog::save
 * @brief Finishes configuring the future source.
 */
void CreateSourceDialog::save() {
  if ((m_dbpath.isEmpty()) or (titleInput->text().isEmpty())) close();
  Source source;
  source.path = m_dbpath;
  source.table_title = titleInput->text();
  QString *table_name = new QString;
  basis->sql->create_source(source, table_name);
  source.table_name = *table_name;
  delete table_name;
  emit add(source);
  close();
}
