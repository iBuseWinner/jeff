#include "sourcelist.h"

/*!
 * @fn SourceList::SourceList
 * @brief The constructor.
 * @param[in,out] parent QObject parent
 */
SourceList::SourceList(QWidget *parent) : QTreeWidget(parent) {
  setHeaderLabel(tr("Sources"));
  setAnimated(false);
  setStyleSheet(
      "SourceList, QHeaderView { background-color: #ffffff; color: #000000; }"
      "QScrollBar:vertical { background: transparent; border: none; width: "
      "5px; margin: 0px 0px 5px 0px; } QScrollBar::handle:vertical { border: "
      "2px solid #e4e4e4; background: #e4e4e4; width: 5px; border-radius: 1px; "
      "} QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { width: "
      "0px; height: 0px; }");
}
