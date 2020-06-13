#include "a_board.h"

/*!
 * Argument: QWidget {*parent}.
 * Creates an ABoard.
 */
ABoard::ABoard(QWidget *parent) : QFrame(parent) {
  setFrameShape(QFrame::StyledPanel);
  setFrameShadow(QFrame::Raised);
  setObjectName(object_name);
  setStyleSheet(style_sheet);
}
