#ifndef A_LINE_H
#define A_LINE_H

#include <QLayout>
#include <QLineEdit>
#include <QWidget>
#include "widgets/a_board.h"
#include "widgets/a_button.h"
#include "widgets/a_lineedit.h"

/*!
 * Class: ALine.
 * Widget for sending user messages.
 */
class ALine : public ABoard {
  Q_OBJECT
 public:
  // Objects:
  AButton *sendButton = nullptr;
  ALineEdit *lineEdit = nullptr;

  // Functions:
  ALine(QWidget *parent = nullptr);

 private:
  Q_DISABLE_COPY(ALine)
};

#endif  // A_LINE_H
