#include "akiwake_dialog.h"

AkiwakeDialog::AkiwakeDialog(QWidget *parent) : QDialog(parent) {
  // Prepares dialog windows for using.
  this->setMinimumSize(300, 150);
  this->resize(600, 450);
  this->mainBoard = new AkiwakeBoard(this);
  auto *mainBoardContainer = new QGridLayout();
  mainBoardContainer->setMargin(6);
  mainBoardContainer->setSpacing(0);
  auto *baseLayout = new QGridLayout();
  baseLayout->setMargin(0);
  baseLayout->setSpacing(0);
  QWidget *themeWidget = new QWidget(this);
  themeWidget->setObjectName("themeWidget");
  themeWidget->setStyleSheet("#themeWidget {"
                             "background-image: url(:/arts/background.jpg);"
                             "background-position: center center;"
                             "background-repeat:  no-repeat;"
                             "background-attachment: fixed;"
                             "}");
  mainBoardContainer->addWidget(this->mainBoard);
  themeWidget->setLayout(mainBoardContainer);
  baseLayout->addWidget(themeWidget);
  this->setLayout(baseLayout);
}
