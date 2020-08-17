#include "a_sourcelist.h"

/*!
 * @fn ASourceList::ASourceList
 * @brief The constructor.
 * @param[in,out] parent QObject parent
 */
ASourceList::ASourceList(QWidget *parent) : QTreeWidget(parent) {
  setHeaderLabel(tr("Sources"));
  setAnimated(false);
}
