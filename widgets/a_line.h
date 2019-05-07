#ifndef A_LINE_H
#define A_LINE_H

#include <QLayout>
#include <QLineEdit>
#include <QWidget>
#include "widgets/a_board.h"
#include "widgets/a_button.h"
#include "widgets/a_lineedit.h"

class ALine : public ABoard {
  Q_OBJECT
 public:
  AButton *s = nullptr;
  ALineEdit *tl = nullptr;
  ALine(QWidget *p = nullptr);

 private:
  Q_DISABLE_COPY(ALine)
};

#endif  // A_LINE_H
