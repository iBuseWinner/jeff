#include "akiwake_board.h"

AkiwakeBoard::AkiwakeBoard(QWidget *parent) : QFrame(parent) {
  // Creates top-level board.
  // Creates main layout...
  this->setFrameShape(QFrame::StyledPanel);
  this->setFrameShadow(QFrame::Raised);
  this->setObjectName("fr");
  this->setStyleSheet("#fr {border-width: 4px;"
                      "border-style: solid;"
                      "border-color: white;"
                      "border-radius: 6px;}"
                      "* {"
                      "background-color: white;"
                      "color: black;"
                      "}");
  auto *Layout = new QGridLayout(this);
  Layout->setSpacing(0);
  Layout->setMargin(2);
  // Creates central widget that can contain anything...
  this->centralWidget->setObjectName("cwd");
  Layout->addWidget(this->centralWidget);
  // Sets the layout...
  this->setLayout(Layout);
}
