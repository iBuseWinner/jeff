#include "a_containerslist.h"

AContainersList::AContainersList(QWidget *p) : QTreeWidget(p) {
  // Creates a special tree widget for displaying containers.
  setHeaderLabel("Containers");
  setAnimated(false);
}
