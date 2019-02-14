#include "a_line.h"

ALine::ALine(QWidget *parent) : ABoard(parent) {
  // Creates a user input widget.
  this->textLine = new ALineEdit(this);
  this->sendButton = new APushButton("Send", this);
  this->sendButton->setFocusPolicy(Qt::NoFocus);
  connect(this->textLine, &QLineEdit::returnPressed, this->sendButton,
          &APushButton::click);
  auto *entireLayout = new QHBoxLayout(this);
  entireLayout->setMargin(0);
  entireLayout->setSpacing(0);
  entireLayout->addWidget(this->textLine);
  entireLayout->addWidget(this->sendButton);
  this->setLayout(entireLayout);
}
