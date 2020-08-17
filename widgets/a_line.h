#ifndef A_LINE_H
#define A_LINE_H

#include "widgets/a_board.h"
#include "widgets/a_button.h"
#include "widgets/a_lineedit.h"
#include <QLayout>
#include <QLineEdit>
#include <QWidget>

/*!
 * @class ALine
 * @brief Widget for sending user messages.
 * @sa ABoard, AButton, ALineEdit
 */
class ALine : public ABoard {
  Q_OBJECT
public:
  // Objects:
  AButton *sendButton = nullptr;
  ALineEdit *lineEdit = nullptr;

  // Functions described in 'a_line.cpp':
  explicit ALine(QWidget *parent = nullptr);

private:
  Q_DISABLE_COPY(ALine)
};

#endif // A_LINE_H
