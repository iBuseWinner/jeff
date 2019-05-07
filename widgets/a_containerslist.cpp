#include "a_containerslist.h"

AContainersList::AContainersList(QWidget *p) : QTreeWidget(p) {
  setHeaderLabel("Containers");
  setAnimated(false);
}
