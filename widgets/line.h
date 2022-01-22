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
public:
  // Objects:
  Button *send_button = nullptr;
  LineEdit *line_edit = nullptr;

  // Functions described in 'line.cpp':
  explicit Line(QWidget *parent = nullptr);

private:
  Q_DISABLE_COPY(Line)
};

#endif
