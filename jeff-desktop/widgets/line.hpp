#ifndef LINE_H
#define LINE_H

#include "widgets/board.hpp"
#include "widgets/button.hpp"
#include "widgets/textedit.hpp"
#include "widgets/layouts/linears.hpp"
#include <QLayout>
#include <QLineEdit>
#include <QWidget>

/*! @class Line
 *  @brief Widget for sending user messages.
 *  @sa Board, Button, LineEdit  */
class Line : public Board {
  Q_OBJECT
  Q_DISABLE_COPY(Line)
public:
  // Objects:
  Button send_button;
  TextEdit line_edit;

  // Functions described in `line.cpp`:
  Line(QWidget *parent = nullptr);
};

#endif
