#include "a_board.h"

/*
 * All short named objects and their explanations:
 * {objn} <- object name
 * {ss} <- style sheet
 */

/*!
 * Argument: QWidget {*parent}.
 * Creates an ABoard.
 */
ABoard::ABoard(QWidget *parent) : QFrame(parent) {
  setFrameShape(QFrame::StyledPanel);
  setFrameShadow(QFrame::Raised);
  setObjectName(objn);
  setStyleSheet(ss);
}
