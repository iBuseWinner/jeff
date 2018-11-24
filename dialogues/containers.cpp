#include "containers.h"

Containers::Containers(QWidget *parent) : AkiwakeDialog(parent) {
  // Creates main objects.
  this->acl = new AkiwakeContainersList();
  this->addContainer = new AkiwakePushButton("Add container");
  this->createContainer = new AkiwakePushButton("Create container");
  this->disconnectContainer = new AkiwakePushButton("Disconnect container");
  this->removeContainer = new AkiwakePushButton("Remove container");
  this->ok = new AkiwakePushButton("Save && Close");
  QGridLayout *entireLayout = new QGridLayout();
  entireLayout->addWidget(this->acl, 0, 0, 1, 0);
  entireLayout->addWidget(this->addContainer, 1, 0);
  entireLayout->addWidget(this->createContainer, 1, 1);
  entireLayout->addWidget(this->disconnectContainer, 1, 2);
  entireLayout->addWidget(this->removeContainer, 1, 3);
  entireLayout->addWidget(this->ok, 1, 4);
  this->mainBoard->centralWidget->setLayout(entireLayout);
  this->loadingFromFile();
  this->connector();
}

Containers::~Containers() {
  // Prevents memory leaks
  delete this->addContainer;
  delete this->createContainer;
  delete this->disconnectContainer;
  delete this->removeContainer;
  delete this->ok;
  delete this->acl;
}

void Containers::loadingFromFile() {
  // Loads containers from saved AS selection.
  SettingsStore *ST = new SettingsStore();
  QList<containerProperties> Set = ST->read();
  delete ST;
  this->appendCLTree(Set);
}

void Containers::connector() {
  // Connects signals with slots.
  connect(this->addContainer, &AkiwakePushButton::clicked, this,
          &Containers::addDB);
  connect(this->createContainer, &AkiwakePushButton::clicked, this,
          &Containers::createDB);
  connect(this->removeContainer, &AkiwakePushButton::clicked, this,
          &Containers::removeDB);
  connect(this->ok, &AkiwakePushButton::clicked, this, &Containers::close);
}

void Containers::addDB() {
  // Adds database and containers into AS selection for searching.
  QString Path = QFileDialog::getOpenFileName(this, "Select database", nullptr,
                                              "SQLite3 database(*.db)");
  sqlite SQ;
  QStringList Containers = SQ.containers(Path);
  QList<containerProperties> Set;
  for (int i = 0; i < Containers.length(); i++) {
    containerProperties CP;
    CP.path = Path;
    CP.container = Containers.at(i);
    Set.append(CP);
  }
  this->appendCLTree(Set);
}

void Containers::createDB() {
  CreateContainer *CC = new CreateContainer();
  CC->exec();
}

void Containers::appendCLTree(QList<containerProperties> Set) {
  // Appends container's table.
  // For every record in "Set" selection...
  for (int i = 0; i < Set.length(); i++) {
    // "element" - Path field...
    QTreeWidgetItem *element = nullptr;
    bool into = false;
    for (int j = 0; j < acl->topLevelItemCount(); j++)
      // If this path already in the tree (because we can have a lot of
      // containers into one database path)...
      if (this->acl->invisibleRootItem()->child(j)->text(0) == Set.at(i).path) {
        into = true;
        element = this->acl->invisibleRootItem()->takeChild(j);
        // ...we links element on it.
        break;
      }
    // If this path isn't in the tree...
    if (!into) {
      element = new QTreeWidgetItem();
      element->setText(0, Set.at(i).path);
    }
    this->acl->addTopLevelItem(element);
    // Now AS can go to add a container:
    bool notContains = true;
    for (int j = 0; j < element->childCount(); j++)
      if (element->child(j)->text(0) == Set.at(i).container) {
        // If this container's name already in the tree and have common path...
        notContains = false;
        break;
      }
    if (notContains) {
      // Adds container:
      QTreeWidgetItem *childElement = new QTreeWidgetItem(element);
      childElement->setText(0, Set.at(i).container);
    }
  }
}

void Containers::removeDB() {
  // Removes a container from the widget.
  for (int i = 0; i < this->acl->selectedItems().length(); i++) {
    QTreeWidgetItem *box;
    if (this->acl->selectedItems().at(i)->parent() == nullptr)
      box = this->acl->invisibleRootItem();
    else
      box = this->acl->selectedItems().at(i)->parent();
    box->removeChild(this->acl->selectedItems().at(i));
    if ((box->childCount() == 0) && (box != this->acl->invisibleRootItem()))
      this->acl->invisibleRootItem()->removeChild(box);
  }
}

void Containers::closeEvent(QCloseEvent *event) {
  // Saves the container's list.
  QList<containerProperties> Set;
  for (int i = 0; i < this->acl->topLevelItemCount(); i++) {
    for (int j = 0; j < this->acl->invisibleRootItem()->child(i)->childCount();
         j++) {
      containerProperties Container;
      Container.path = this->acl->invisibleRootItem()->child(i)->text(0);
      Container.container =
          this->acl->invisibleRootItem()->child(i)->child(j)->text(0);
      Set.append(Container);
    }
  }
  SettingsStore *ST = new SettingsStore();
  ST->write(Set);
  delete ST;
  event->accept();
}
