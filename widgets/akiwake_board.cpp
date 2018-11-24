#include "akiwake_board.h"

AkiwakeBoard::AkiwakeBoard(QWidget *parent) : QWidget(parent) {
  // Creates top-level board.
  // Creates main layout...
  QGridLayout *Layout = new QGridLayout();
  Layout->setSpacing(0);
  Layout->setContentsMargins(0, 0, 0, 0);
  // Creates upper left corner...
  this->LH->setStyleSheet(this->borderTheme(0, 1));
  this->LH->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  this->LH->setMinimumSize(16, 8);
  Layout->addWidget(this->LH, 0, 0);
  // Creates upper bound...
  this->H->setStyleSheet(this->borderTheme(0, 2));
  this->H->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
  this->H->setMinimumSize(8, 8);
  Layout->addWidget(this->H, 0, 1);
  // Creates upper right corner...
  this->RH->setStyleSheet(this->borderTheme(0, 3));
  this->RH->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  this->RH->setMinimumSize(16, 8);
  Layout->addWidget(this->RH, 0, 2);
  // Creates left bound...
  this->L->setStyleSheet(this->borderTheme(0, 4));
  this->L->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
  this->L->setMinimumSize(16, 8);
  Layout->addWidget(this->L, 1, 0);
  // Creates central widget that can contain anything...
  this->centralWidget->setObjectName("cwd");
  this->centralWidget->setStyleSheet("QWidget QLineEdit, #cwd{" +
                                     this->borderTheme(0, 2) + "}");
  Layout->addWidget(this->centralWidget, 1, 1);
  // Creates right bound...
  this->R->setStyleSheet(this->borderTheme(0, 5));
  this->R->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
  this->R->setMinimumSize(16, 8);
  Layout->addWidget(this->R, 1, 2);
  // Creates lower left corner...
  this->LL1->setStyleSheet(this->borderTheme(0, 6));
  this->LL1->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  this->LL1->setMinimumSize(16, 8);
  Layout->addWidget(this->LL1, 2, 0);
  // Creates lower bound...
  this->L1->setStyleSheet(this->borderTheme(0, 2));
  this->L1->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
  this->L1->setMinimumSize(8, 8);
  Layout->addWidget(this->L1, 2, 1);
  // Creates lower right corner...
  this->RL1->setStyleSheet(this->borderTheme(0, 7));
  this->RL1->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  this->RL1->setMinimumSize(16, 8);
  Layout->addWidget(this->RL1, 2, 2);
  // Sets the layout...
  this->setLayout(Layout);
}

QString AkiwakeBoard::borderTheme(qint8 Folder, qint8 ID) {
  // Assigns style sheets.
  if ((ID != 6) && (ID != 7)) {
    return QString("border-image: url(:/arts/messages/%1/msg_%2.png);")
        .arg(Folder)
        .arg(ID);
  }
  return QString("border-image: url(:/arts/messages/%1/msg_0_%2.png);")
      .arg(Folder)
      .arg(ID);
}

QString AkiwakeBoard::borderTheme(qint8 Folder, qint8 ID, bool Swap) {
  // Assigns style sheets.
  if ((ID != 6) && (ID != 7)) {
    return QString("border-image: url(:/arts/messages/%1/msg_%2.png);")
        .arg(Folder)
        .arg(ID);
  }
  // Assigns a special style sheets to some AkiwakeMessage's corners...
  return QString("border-image: url(:/arts/messages/%1/msg_%2_%3.png);")
      .arg(Folder)
      .arg(Swap)
      .arg(ID);
}
