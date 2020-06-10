#include "a_expressionlist.h"

AExpressionList::AExpressionList(QWidget *parent) : QListWidget(parent) {
  setWordWrap(true);
  setSelectionRectVisible(true);
  setSelectionMode(QAbstractItemView::ExtendedSelection);
  setDragDropMode(QAbstractItemView::DragDrop);
  setDefaultDropAction(Qt::MoveAction);
}
