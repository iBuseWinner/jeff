#include "a_message.h"

#include <QDebug>
#include <utility>

AMessage::AMessage(QWidget *parent) : QWidget(parent) {
  // Creates a message.
  this->entireLayout->setContentsMargins(0, 0, 0, 6);
  this->entireLayout->setSpacing(0);
  this->setLayout(this->entireLayout);
}

void AMessage::setAuthor(A Author) {
  if (this->returnAuthor() != A::undefA)
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
  auto *spacer = new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding,
                                 QSizePolicy::Minimum);
  this->entireLayout->addWidget(this->board); // content is on the left
  this->entireLayout->addItem(spacer);
  this->update();
}

void AMessage::createStdLayout_user() {
  auto *spacer = new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding,
                                 QSizePolicy::Minimum);
  this->entireLayout->addItem(spacer);
  this->entireLayout->addWidget(this->board); // content is on the right
  this->update();
}

void AMessage::setMessageType(MT Type, QString Content) {
  if (this->returnMessageType() != MT::undefMT)
    return;
  this->m_contentType = Type;
  switch (this->returnMessageType()) {
  case 1:
    this->setupMessage_plain(Content);
    break;
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

void AMessage::setupMessage_plain(const QString &text) {
  this->m_content = text;
  auto *label = new QLabel(text, this);
  label->setTextFormat(Qt::PlainText);
  auto *boardLayout = new QGridLayout();
  boardLayout->addWidget(label);
  this->board->setLayout(boardLayout);
  this->text_label = label;
}

void AMessage::setupMessage_html(const QString &text) {
  this->m_content = text;
  auto *label = new QLabel(text, this);
  label->setTextFormat(Qt::RichText);
  auto *boardLayout = new QGridLayout();
  boardLayout->addWidget(label);
  this->board->setLayout(boardLayout);
  this->text_label = label;
}

void AMessage::setupMessage_widget(QWidget *widget) {
  auto *boardLayout = new QGridLayout();
  boardLayout->addWidget(widget);
  this->board->setLayout(boardLayout);
  this->board->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

// void AMessage::setupMessage_pict(QString path) {}

// void AMessage::setupMessage_file(QString path) {}

QString AMessage::returnText() {
  if (this->returnMessageType() != AMessage::Widget)
    return this->m_content;
  return "";
}

void AMessage::appendLabel(QString Text) {
  // Transfer part of the text to a new line.
  this->text_label->setText(this->text_label->text() + "\n" + Text.trimmed());
}

void AMessage::alignTextToWindowWidth(int MaxWidth) {
  if ((this->m_contentType != MT::Text) && (this->m_contentType != MT::HTML))
    return;
  // Makes messages more readable no matter what text is written.
  // Takes metrics from message label font...
  QFontMetrics Metrics = QFontMetrics(QFont());
  QString Text = this->m_content;
  Text.replace("\n", " \n ");
  Text.replace("<br>", " \n ");
  // Counts width of all text...
  int TextWidth = Metrics.width(Text);
  if (TextWidth + 60 <= MaxWidth) {
    this->text_label->setText(this->m_content);
    return;
  }
  // If message is wider than window width...
  this->text_label->clear();
  while (TextWidth + 60 > MaxWidth) {
    QString SingleLine = "";
    if (Text.split(" ").isEmpty()) {
      break;
    }
    QStringList Words = Text.split(" ");
    for (int index = 0; index < Words.length(); index++) {
      // If we have a very large word...
      if (Metrics.width(Words.at(index)) + 60 > MaxWidth) {
        this->appendLabel(SingleLine);
        Text = Text.remove(0, SingleLine.length()).simplified();
        SingleLine.clear();
        // We eliding it...
        QString Elided =
            Metrics.elidedText(Words.at(index), Qt::ElideRight, MaxWidth - 80);
        this->appendLabel(Elided);
        Text = Text.remove(0, Words.at(index).length()).trimmed();
        TextWidth = Metrics.width(Text);
        continue;
      }
      if (Metrics.width(SingleLine + Words.at(index)) + 60 >= MaxWidth)
        break;
      SingleLine += " " + Words.at(index);
      if (Words.at(index) == "\n")
        break;
    }
    // Cuts the remaining text...
    Text = Text.remove(0, SingleLine.length());
    SingleLine = SingleLine.trimmed();
    // Gets new metrics...
    TextWidth = Metrics.width(Text);
    this->appendLabel(SingleLine);
  }
  this->appendLabel(Text);
  this->text_label->setText(this->text_label->text().trimmed());
  if (this->m_contentType == MT::HTML)
    this->text_label->setText(this->text_label->text().replace("\n", "<br>"));
}
