#include "createcontainer.h"

CreateContainer::CreateContainer(QWidget *parent) : QWidget(parent) {
  this->setAttribute(Qt::WA_DeleteOnClose);
  // Creates main objects...
  auto *entireLayout = new QGridLayout();
  this->containerName = new ALineEdit(this);
  this->containerName->setPlaceholderText("Enter container's name");
  this->save = new APushButton("Save");
  auto *lineSpacer =
      new QSpacerItem(0, 0, QSizePolicy::Fixed, QSizePolicy::Expanding);
  auto *inlineSpacer =
      new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Fixed);
  entireLayout->addWidget(this->containerName, 0, 0, 1, 0);
  entireLayout->addItem(lineSpacer, 1, 0, 1, 0);
  entireLayout->addItem(inlineSpacer, 2, 0);
  entireLayout->addWidget(this->save, 2, 1);
  this->setLayout(entireLayout);
}
