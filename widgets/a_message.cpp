#include "a_message.h"

#include <utility>

AMessage::AMessage(QWidget *parent) : QWidget(parent) {
  // Creates a message.
  this->entireLayout->setContentsMargins(0, 0, 0, 6);
  this->entireLayout->setSpacing(0);
  this->setLayout(this->entireLayout);
}

void AMessage::setAuthor(AT Author) {
  if (this->returnAuthor() != AT::undefAT)
    return;
  this->m_author = Author;
  switch (this->returnAuthor()) {
  case 1:
    this->createStdLayout_asw();
    break;
  case 2:
    this->createStdLayout_user();
    break;
  default:;
  }
}

void AMessage::createStdLayout_asw() {
  auto *spacer =
      new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
  this->entireLayout->addWidget(this->board); // content is on the left
  this->entireLayout->addItem(spacer);
  this->update();
}

void AMessage::createStdLayout_user() {
  auto *spacer =
      new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
  this->entireLayout->addItem(spacer);
  this->entireLayout->addWidget(this->board); // content is on the right
  this->update();
}

void AMessage::setMessageType(MT Type, QString Content) {
  if (this->returnMessageType() != MT::undefMT)
    return;
  this->m_contentType = Type;
  switch (this->returnMessageType()) {
  // case 1:
  //   this->setupMessage_plain(Content);
  //   break;
  case 2:
    this->setupMessage_html(std::move(Content));
    break;
  // case 4:
  //   this->setupMessage_pict(Content);
  //   break;
  // case 5:
  //   this->setupMessage_file(Content);
  //   break;
  default:;
  }
}

void AMessage::setMessageType(MT Type, QWidget *Content) {
  if (this->returnMessageType() != MT::undefMT)
    return;
  this->m_contentType = Type;
  this->setupMessage_widget(Content);
}

// void AMessage::setupMessage_plain(QString text) {}

void AMessage::setupMessage_html(const QString& text) {
  this->m_content = text;
  auto *label = new QLabel(text, this);
  auto *boardLayout = new QGridLayout();
  boardLayout->addWidget(label);
  this->board->setLayout(boardLayout);
}

void AMessage::setupMessage_widget(QWidget *widget) {
  auto *boardLayout = new QGridLayout();
  boardLayout->addWidget(widget);
  this->board->setLayout(boardLayout);
}

// void AMessage::setupMessage_pict(QString path) {}

// void AMessage::setupMessage_file(QString path) {}

QString AMessage::returnText() {
  if (this->returnMessageType() != AMessage::Widget)
    return this->m_content;
  return "";
}
