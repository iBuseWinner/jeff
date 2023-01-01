#ifndef BOARD_H
#define BOARD_H

#include <QFrame>
#include <QGridLayout>
#include <QWidget>
#include "widgets/styling.h"

/*! @class Board
 *  @brief Shell widget. Looks like a white rectangle with rounded edges.  */
class Board : public QFrame {
  Q_OBJECT
  Q_DISABLE_COPY(Board)
public:
  // Constants:
  const char *white_style =
    "#board { border-width: 4px; border-style: solid; border-color: #fcfcfc;"
    "border-radius: 6px; background-color: #fcfcfc; } * { color: #16151d; }";
  const char *black_style =
    "#board { border-width: 4px; border-style: solid; border-color: #16151d;"
    "border-radius: 6px; background-color: #16151d; } * { color: #fcfcfc; }";
  const char *red_style =
    "#board { border-width: 4px; border-style: solid; border-color: #d22a2a;"
    "border-radius: 6px; background-color: #d22a2a; } * { color: #ffffff; }";
  const char *green_style =
    "#board { border-width: 4px; border-style: solid; border-color: #3b961a;"
    "border-radius: 6px; background-color: #3b961a; } * { color: #ffffff; }";
  const char *blue_style =
    "#board { border-width: 4px; border-style: solid; border-color: #2a73a0;"
    "border-radius: 6px; background-color: #2a73a0; } * { color: #ffffff; }";
  const char *yellow_style =
    "#board { border-width: 4px; border-style: solid; border-color: #b7b113;"
    "border-radius: 6px; background-color: #b7b113; } * { color: #2d2d2d; }";
  const char *warning_style =
    "#board { border-width: 4px; border-style: solid; border-color: #ffec4d;"
    "border-radius: 6px; background-color: #ffec4d; } * { color: black; }";
  const char *error_style =
    "#board { border-width: 4px; border-style: solid; border-color: #e8553a;"
    "border-radius: 6px; background-color: #e8553a; } * { color: white; }";

  // Functions described in `board.cpp`:
  Board(QWidget *parent = nullptr);

private:
  // Constants:
  const char *object_name = "board";
};

#endif
