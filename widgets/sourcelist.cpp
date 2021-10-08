#include "sourcelist.h"

/*!
 * @fn SourceList::SourceList
 * @brief The constructor.
 * @param[in,out] parent QObject parent
 */
SourceList::SourceList(QWidget *parent) : QTreeWidget(parent) {
  setHeaderLabel(tr("Sources"));
  setAnimated(false);
}
