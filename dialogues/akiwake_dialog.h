#ifndef AKIWAKE_DIALOG_H
#define AKIWAKE_DIALOG_H

#include "widgets/akiwake_board.h"
#include <QDialog>

class AkiwakeDialog : public QDialog {
  Q_OBJECT
public:
  explicit AkiwakeDialog(QWidget *parent = nullptr);
  AkiwakeBoard *mainBoard;
};

#endif // AKIWAKE_DIALOG_H
