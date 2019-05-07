#include "createcontainer.h"

CreateContainer::CreateContainer(QWidget *parent) : QWidget(parent) {
  auto *el = new QGridLayout();
  t = new ALineEdit(this);
  sel = new AButton("", this);
  s = new AButton("Save", this);
  cc = new AButton("Cancel", this);
  auto *is = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Fixed);
  t->setPlaceholderText("Enter container's name");
  s->setIcon(QIcon(":/arts/icons/16/document-save.svg"));
  cc->setIcon(QIcon(":/arts/icons/16/window-close.svg"));
  el->addWidget(t, 0, 0, 1, 0);
  el->addWidget(sel, 1, 0, 1, 0);
  el->addItem(is, 2, 0);
  el->addWidget(s, 2, 1);
  el->addWidget(cc, 2, 2);
  setLayout(el);
  connector();
  selStart();
}

void CreateContainer::closeEvent(QCloseEvent *e) {
  emit c();
  e->accept();
}

void CreateContainer::connector() {
  connect(sel, &AButton::clicked, this, &CreateContainer::select);
  connect(s, &AButton::clicked, this, &CreateContainer::save);
  connect(cc, &AButton::clicked, this, &QWidget::close);
}

void CreateContainer::selStart() {
  sel->setText("Select database file...");
  sel->setIcon(QIcon(":/arts/icons/16/document-open.svg"));
}

void CreateContainer::select() {
  m_dbpath = QFileDialog::getSaveFileName(nullptr, "Select database...", "",
                                          "ASW database(*.asw.db)", nullptr,
                                          QFileDialog::DontConfirmOverwrite);
  if (!m_dbpath.isEmpty()) {
    sel->setText(m_dbpath);
    sel->setIcon(QIcon(":/arts/icons/16/db-file.svg"));
  } else
    selStart();
}

void CreateContainer::save() {
  if ((m_dbpath.isEmpty()) || (t->text().isEmpty())) close();
  container cProp;
  QRandomGenerator rand(quint32(QTime::currentTime().msec()));
  cProp.p = m_dbpath;
  cProp.t = t->text();
  cProp.c = QUuid(uint(rand.generate()), ushort(rand.generate()),
                  ushort(rand.generate()), uchar(rand.bounded(0, 256)),
                  uchar(rand.bounded(0, 256)), uchar(rand.bounded(0, 256)),
                  uchar(rand.bounded(0, 256)), uchar(rand.bounded(0, 256)),
                  uchar(rand.bounded(0, 256)), uchar(rand.bounded(0, 256)),
                  uchar(rand.bounded(0, 256)))
                .toString();
  emit cont(cProp);
  close();
}
