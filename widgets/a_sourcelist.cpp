#include "a_sourcelist.h"

/*!
 * Argument: QWidget {*parent}.
 * Creates an ASourceList.
 */
ASourceList::ASourceList(QWidget *parent) : QTreeWidget(parent) {
  setHeaderLabel(tr("Sources"));
  setAnimated(false);
}
