#ifndef BOARD_H
#define BOARD_H

#include <QFrame>
#include <QGridLayout>
#include <QWidget>

/*!
 * @class Board
 * @brief Shell widget. Looks like a white rectangle with rounded edges.
 */
class Board : public QFrame {
  Q_OBJECT
public:
  // Functions described in 'a_board.cpp':
  explicit Board(QWidget *parent = nullptr);

  // Constants:
  inline static const QString warning_style = "#board {"
                                              "border-width: 4px;"
                                              "border-style: solid;"
                                              "border-color: #ffec4d;"
                                              "border-radius: 6px;"
                                              "background-color: #ffec4d;"
                                              "}"
                                              "* {"
                                              "color: black;"
                                              "}";
  inline static const QString error_style = "#board {"
                                            "border-width: 4px;"
                                            "border-style: solid;"
                                            "border-color: #e8553a;"
                                            "border-radius: 6px;"
                                            "background-color: #e8553a;"
                                            "}"
                                            "* {"
                                            "color: white;"
                                            "}";

private:
  Q_DISABLE_COPY(Board)

  // Constants:
  inline static const QString object_name = "board";
  inline static const QString style_sheet = "#board {"
                                            "border-width: 4px;"
                                            "border-style: solid;"
                                            "border-color: white;"
                                            "border-radius: 6px;"
                                            "background-color: white;"
                                            "}"
                                            "* {"
                                            "color: black;"
                                            "}";
};

#endif
