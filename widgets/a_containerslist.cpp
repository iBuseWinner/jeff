#include "a_containerslist.h"

/*!
 * Argument: QWidget {*parent}.
 * Creates an AContainerList.
 */
AContainersList::AContainersList(QWidget *parent) : QTreeWidget(parent) {
  setHeaderLabel(tr("Containers"));
  setAnimated(false);
}
