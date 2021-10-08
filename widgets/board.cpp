#include "board.h"

/*!
 * @fn Board::Board
 * @brief The constructor.
 * @param[in,out] parent QObject parent
 */
Board::Board(QWidget *parent) : QFrame(parent) {
  setFrameShape(QFrame::StyledPanel);
  setFrameShadow(QFrame::Raised);
  setObjectName(object_name);
  setStyleSheet(style_sheet);
}
