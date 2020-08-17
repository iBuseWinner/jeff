#include "a_expressionlist.h"

/*!
 * @fn AExpressionList::AExpressionList
 * @brief The constructor.
 * @param[in,out] parent QObject parent
 */
AExpressionList::AExpressionList(QWidget *parent) : QListWidget(parent) {
  setWordWrap(true);
  setSelectionRectVisible(true);
  setSelectionMode(QAbstractItemView::ExtendedSelection);
  setDragDropMode(QAbstractItemView::DragDrop);
  setDefaultDropAction(Qt::MoveAction);
}
