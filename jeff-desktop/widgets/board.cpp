#include "board.hpp"

/*! @brief The constructor. */
Board::Board(QWidget *parent) : QFrame(parent) {
  setFrameShape(QFrame::StyledPanel);
  setFrameShadow(QFrame::Raised);
  setObjectName(object_name);
  setStyleSheet(QString(
    "#board { border-width: 4px; border-style: solid; border-color: %1;"
    "border-radius: 6px; background-color: %1; } * { color: %2; }"
  ).arg(styling.css_bg_color).arg(styling.css_fg_color));
}
