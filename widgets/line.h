#ifndef LINE_H
#define LINE_H

#include "widgets/board.h"
#include "widgets/button.h"
#include "widgets/lineedit.h"
#include <QLayout>
#include <QLineEdit>
#include <QWidget>

/*!
 * @class Line
 * @brief Widget for sending user messages.
 * @sa Board, Button, LineEdit
 */
class Line : public Board {
  Q_OBJECT
  Q_DISABLE_COPY(Line)
public:
  // Objects:
  Button send_button;
  LineEdit line_edit;

  // Functions described in `line.cpp`:
  Line(QWidget *parent = nullptr);

private:
  //Objects:
  QHBoxLayout layout;
};

#endif
