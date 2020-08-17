#include "a_board.h"

/*!
 * @fn ABoard::ABoard
 * @brief The constructor.
 * @param[in,out] parent QObject parent
 */
ABoard::ABoard(QWidget *parent) : QFrame(parent) {
  setFrameShape(QFrame::StyledPanel);
  setFrameShadow(QFrame::Raised);
  setObjectName(object_name);
  setStyleSheet(style_sheet);
}
