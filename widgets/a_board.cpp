#include "a_board.h"

ABoard::ABoard(QWidget *parent) : QFrame(parent) {
  setFrameShape(QFrame::StyledPanel);
  setFrameShadow(QFrame::Raised);
  setObjectName(objn);
  setStyleSheet(ss);
}
