#ifndef A_BOARD_H
#define A_BOARD_H

#include <QFrame>
#include <QGridLayout>
#include <QWidget>

/*!
 * Class: ABoard
 * Shell widget. Looks like a white rectangle with rounded edges.
 */
class ABoard : public QFrame {
  Q_OBJECT
 public:
  // Functions:
  ABoard(QWidget *parent = nullptr);

 private:
  Q_DISABLE_COPY(ABoard)

  // Objects:
  QString objn = "a_board";
  QString ss =
      "#a_board {border-width: 4px; border-style: solid; border-color: white; "
      "border-radius: 6px; background-color: white;} * { color: black; }";
};

#endif
