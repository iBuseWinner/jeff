#include "createcontainer.h"

/*
 * All short named objects and their explanations:
 * {lt} <- layout
 * {sp} <- spacer
 */

/*!
 * Argument: QWidget {*parent}.
 * Constructs and prepares Create Container.
 */
CreateContainer::CreateContainer(QWidget *parent) : QWidget(parent) {
  auto *lt = new QGridLayout();
  titleInput = new ALineEdit(this);
  selectFileBtn = new AButton("", this);
  saveBtn = new AButton(tr("Save"), this);
  cancelBtn = new AButton(tr("Cancel"), this);
  auto *sp = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Fixed);
  titleInput->setPlaceholderText(tr("Enter container's name"));
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
void CreateContainer::connector() {
  connect(selectFileBtn, &AButton::clicked, this, &CreateContainer::select);
  connect(saveBtn, &AButton::clicked, this, &CreateContainer::save);
  connect(cancelBtn, &AButton::clicked, this, &QWidget::close);
}

/*! Turns the faceless button into a button for selecting a database. */
void CreateContainer::selStart() {
  selectFileBtn->setText(tr("Select database file..."));
  selectFileBtn->setIcon(QIcon(":/arts/icons/16/document-open.svg"));
}

/*! Opens the database selection window and customizes the button. */
void CreateContainer::select() {
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
void CreateContainer::save() {
  if ((m_dbpath.isEmpty()) || (titleInput->text().isEmpty())) close();
  container cProp;
  cProp.path = m_dbpath;
  cProp.tableTitle = titleInput->text();
  emit completed(cProp);
  close();
}
