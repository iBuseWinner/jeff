#include "createcontainer.h"

CreateContainer::CreateContainer(QWidget *parent) : QWidget(parent) {
  this->setAttribute(Qt::WA_DeleteOnClose);
  // Creates main objects...
  auto *entireLayout = new QGridLayout();
  this->containerName = new ALineEdit(this);
  this->containerName->setPlaceholderText("Enter container's name");
  this->save = new APushButton("Save");
  this->cancel = new APushButton("Cancel");
  auto *inlineSpacer =
      new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Fixed);
  entireLayout->addWidget(this->containerName, 0, 0, 1, 0);
  entireLayout->addItem(inlineSpacer, 1, 0);
  entireLayout->addWidget(this->save, 1, 1);
  entireLayout->addWidget(this->cancel, 1, 2);
  this->setLayout(entireLayout);
  this->connector();
}

void CreateContainer::connector() {
  connect(this->cancel, &APushButton::clicked, this, &QWidget::close);
}

void CreateContainer::closeEvent(QCloseEvent *event) {
  emit closingSignal();
  event->accept();
}
