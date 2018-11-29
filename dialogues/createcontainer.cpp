#include "createcontainer.h"

CreateContainer::CreateContainer(QWidget *parent) : AkiwakeDialog(parent) {
  this->containerName = new AkiwakeLineEdit();
  this->containerName->setPlaceholderText("Enter container's name");
  this->save = new AkiwakePushButton("Save");
  QGridLayout *entireLayout = new QGridLayout();
  QSpacerItem *lineSpacer =
      new QSpacerItem(0, 0, QSizePolicy::Fixed, QSizePolicy::Expanding);
  QSpacerItem *inlineSpacer =
      new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Fixed);
  entireLayout->addWidget(this->containerName, 0, 0, 1, 0);
  entireLayout->addItem(lineSpacer, 1, 0, 1, 0);
  entireLayout->addItem(inlineSpacer, 2, 0);
  entireLayout->addWidget(this->save, 2, 1);
  this->mainBoard->centralWidget->setLayout(entireLayout);
}
