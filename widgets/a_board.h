#ifndef A_BOARD_H
#define A_BOARD_H

#include <QFrame>
#include <QGridLayout>
#include <QWidget>

/*!
 * @class ABoard
 * @brief Shell widget. Looks like a white rectangle with rounded edges.
 */
class ABoard : public QFrame {
  Q_OBJECT
public:
  // Functions described in 'a_board.cpp':
  explicit ABoard(QWidget *parent = nullptr);

  // Constants:
  inline static const QString warning_style = "#a_board {"
                                              "border-width: 4px;"
                                              "border-style: solid;"
                                              "border-color: #ffec4d;"
                                              "border-radius: 6px;"
                                              "background-color: #ffec4d;"
                                              "}"
                                              "* {"
                                              "background-color: white;"
                                              "color: white;"
                                              "}";
  inline static const QString error_style = "#a_board {"
                                            "border-width: 4px;"
                                            "border-style: solid;"
                                            "border-color: #e8553a;"
                                            "border-radius: 6px;"
                                            "background-color: #e8553a;"
                                            "}"
                                            "* {"
                                            "background-color: white;"
                                            "color: white;"
                                            "}";

private:
  Q_DISABLE_COPY(ABoard)

  // Constants:
  inline static const QString object_name = "a_board";
  inline static const QString style_sheet = "#a_board {"
                                            "border-width: 4px;"
                                            "border-style: solid;"
                                            "border-color: white;"
                                            "border-radius: 6px;"
                                            "background-color: white;"
                                            "}"
                                            "* {"
                                            "background-color: white;"
                                            "color: black;"
                                            "}";
};

#endif
