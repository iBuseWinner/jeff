#ifndef AKIWAKE_LINE_H
#define AKIWAKE_LINE_H

#include "widgets/akiwake_board.h"
#include "widgets/akiwake_lineedit.h"
#include "widgets/akiwake_pushbutton.h"
#include <QFontDatabase>
#include <QLayout>
#include <QLineEdit>
#include <QWidget>

class AkiwakeLine : public AkiwakeBoard {
  Q_OBJECT
public:
  explicit AkiwakeLine(QWidget *parent = nullptr);
  AkiwakeLineEdit *textLine;
  AkiwakePushButton *sendButton;

private:
  Q_DISABLE_COPY(AkiwakeLine)
};

#endif // AKIWAKE_LINE_H
