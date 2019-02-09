#include "createcontainer.h"

CreateContainer::CreateContainer(QWidget *parent) : AkiwakeDialog(parent) {
  this->setWindowTitle("Create container");
  // Creates main objects...
  auto *entireLayout = new QGridLayout();
  this->containerName = new AkiwakeLineEdit(this);
  this->containerName->setPlaceholderText("Enter container's name");
  this->save = new AkiwakePushButton("Save");
  auto *lineSpacer =
      new QSpacerItem(0, 0, QSizePolicy::Fixed, QSizePolicy::Expanding);
  auto *inlineSpacer =
      new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Fixed);
  entireLayout->addWidget(this->containerName, 0, 0, 1, 0);
  entireLayout->addItem(lineSpacer, 1, 0, 1, 0);
  entireLayout->addItem(inlineSpacer, 2, 0);
  entireLayout->addWidget(this->save, 2, 1);
  this->mainBoard->centralWidget->setLayout(entireLayout);
}
