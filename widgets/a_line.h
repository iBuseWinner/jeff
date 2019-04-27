#ifndef A_LINE_H
#define A_LINE_H

#include <QFontDatabase>
#include <QLayout>
#include <QLineEdit>
#include <QWidget>
#include "widgets/a_board.h"
#include "widgets/a_lineedit.h"
#include "widgets/a_pushbutton.h"

class ALine : public ABoard {
  Q_OBJECT
 public:
  ALine(QWidget *p = nullptr);
  ALineEdit *tl;
  APushButton *s;

 private:
  Q_DISABLE_COPY(ALine)
};

#endif  // A_LINE_H
