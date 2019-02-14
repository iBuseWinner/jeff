#include "a_board.h"

ABoard::ABoard(QWidget *parent) : QFrame(parent) {
  // Creates top-level board.
  this->setFrameShape(QFrame::StyledPanel);
  this->setFrameShadow(QFrame::Raised);
  this->setObjectName(this->objectName);
  this->setStyleSheet(this->css);
}
