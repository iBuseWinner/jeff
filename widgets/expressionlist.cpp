#include "expressionlist.h"

/*!
 * @fn ExpressionList::ExpressionList
 * @brief The constructor.
 * @param[in,out] parent QObject parent
 */
ExpressionList::ExpressionList(QWidget *parent) : QListWidget(parent) {
  setWordWrap(true);
  setSelectionRectVisible(true);
  setSelectionMode(QAbstractItemView::ExtendedSelection);
  setDragDropMode(QAbstractItemView::DragDrop);
  setDefaultDropAction(Qt::MoveAction);
}
