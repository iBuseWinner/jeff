#include "create-source.h"

/*
 * All short named objects and their explanations:
 * {lt} <- layout
 * {sp} <- spacer
 */

/*!
 * Argument: QWidget {*parent}.
 * Constructs and prepares Create Source dialogue.
 */
CreateSourceDialog::CreateSourceDialog(Basis *_basis, QWidget *parent)
    : QWidget(parent) {
  basis = _basis;
  auto *lt = new QGridLayout();
  titleInput = new ALineEdit(this);
  selectFileBtn = new AButton("", this);
  saveBtn = new AButton(tr("Save"), this);
  cancelBtn = new AButton(tr("Cancel"), this);
  auto *sp = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Fixed);
  titleInput->setPlaceholderText(tr("Enter source name"));
  saveBtn->setIcon(QIcon(":/arts/icons/16/document-save.svg"));
  cancelBtn->setIcon(QIcon(":/arts/icons/16/window-close.svg"));
  lt->addWidget(titleInput, 0, 0, 1, 0);
  lt->addWidget(selectFileBtn, 1, 0, 1, 0);
  lt->addItem(sp, 2, 0);
  lt->addWidget(saveBtn, 2, 1);
  lt->addWidget(cancelBtn, 2, 2);
  setLayout(lt);
  connector();
  selStart();
}

/*! Establishes communications for user interaction through the widget. */
void CreateSourceDialog::connector() {
  connect(selectFileBtn, &AButton::clicked, this, &CreateSourceDialog::select);
  connect(saveBtn, &AButton::clicked, this, &CreateSourceDialog::save);
  connect(cancelBtn, &AButton::clicked, this, &QWidget::close);
}

/*! Turns the faceless button into a button for selecting a database. */
void CreateSourceDialog::selStart() {
  selectFileBtn->setText(tr("Select database file..."));
  selectFileBtn->setIcon(QIcon(":/arts/icons/16/document-open.svg"));
}

/*! Opens the database selection window and customizes the button. */
void CreateSourceDialog::select() {
  m_dbpath = QFileDialog::getSaveFileName(
      nullptr, tr("Select database..."), "", tr("ASW database") + "(*.asw.db)",
      nullptr, QFileDialog::DontConfirmOverwrite);
  if (!m_dbpath.isEmpty()) {
    selectFileBtn->setText(m_dbpath);
    selectFileBtn->setIcon(QIcon(":/arts/icons/16/db-file.svg"));
  } else
    selStart();
}

/*! Finishes configuring the future container. */
void CreateSourceDialog::save() {
  if ((m_dbpath.isEmpty()) || (titleInput->text().isEmpty()))
    close();
  Source cProp;
  cProp.path = m_dbpath;
  cProp.tableTitle = titleInput->text();
  basis->sql->create(cProp);
  cProp.tableName = basis->sql->getUuid();
  emit add(cProp);
  close();
}
