#include "akiwake_message.h"
#include <QDebug>

AkiwakeMessage::AkiwakeMessage(QString Text, AuthorType Author,
                               ThemeType Theme = AkiwakeMessage::Light,
                               QWidget *parent)
    : QWidget(parent) {
  // Creates a message.
  // Variable assignment...
  this->themeFolder = Theme;
  this->messageAuthor = Author;
  this->messageLabel = this->getMessageLabel(Text);
  this->labelTextColor();
  this->themeUpdater();
  // Creating layout for adding message into AkiwakeBoard...
  QLayout *labelLayout = new QGridLayout();
  labelLayout->setContentsMargins(0, 0, 0, 0);
  labelLayout->setSpacing(0);
  labelLayout->addWidget(this->messageLabel);
  this->board->centralWidget->setLayout(labelLayout);
  // Creating message output format...
  QHBoxLayout *line = new QHBoxLayout();
  line->setContentsMargins(0, 0, 0, 6);
  line->setSpacing(0);
  QSpacerItem *lineSpacer =
      new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Fixed);
  if (this->messageAuthor == AkiwakeMessage::ASW) {
    line->addWidget(this->board);
    line->addSpacerItem(lineSpacer);
  } else {
    line->addSpacerItem(lineSpacer);
    line->addWidget(this->board);
  }
  this->setLayout(line);
  // Animation block...
  this->Animation = new QPropertyAnimation(this, "maximumHeight");
  this->Animation->setDuration(1000);
  this->Animation->setStartValue(0);
  this->Animation->setEndValue(1000);
  this->Animation->setEasingCurve(QEasingCurve::InOutQuad);
  this->Animation->start();
}

AkiwakeMessage::~AkiwakeMessage() {
  delete this->Animation;
}

QLabel *AkiwakeMessage::getMessageLabel(QString Text) {
  // Creates the message label.
  QLabel *Message = new QLabel(Text);
  int ID = QFontDatabase::addApplicationFont(
      ":/arts/fonts/fira-sans-condensed-regular.ttf");
  QFont Font = QFontDatabase::applicationFontFamilies(ID).at(0);
  Font.setPixelSize(18);
  Message->setFont(Font);
  Message->setTextFormat(Qt::RichText);
  this->currentText = Text;
  Message->setTextInteractionFlags(Qt::TextSelectableByMouse |
                                   Qt::LinksAccessibleByMouse);
  Message->setFocusPolicy(Qt::NoFocus);
  Message->setOpenExternalLinks(true);
  return Message;
}

void AkiwakeMessage::labelTextColor() {
  // Changes the font color depending on the message theme.
  if ((this->themeFolder != ThemeType::Dark) &&
      (this->themeFolder != ThemeType::Blue) &&
      (this->themeFolder != ThemeType::Red))
    this->messageLabel->setStyleSheet("color: rgb(0, 0, 0);");
  else
    this->messageLabel->setStyleSheet("color: rgb(255, 255, 255);");
}

QString AkiwakeMessage::returnCurrentText() {
  // Returns plain text of the message.
  return this->currentText;
}

void AkiwakeMessage::labelAppending(QString Text) {
  // Transfer part of the text to a new line.
  this->messageLabel->setText(this->messageLabel->text() + "\n" +
                              Text.simplified());
  this->messageLabel->setText(this->messageLabel->text().trimmed());
}

void AkiwakeMessage::textLayoutDesigner(int width) {
  // Makes messages more readable no matter what text is written.
  // Takes metrics from message label font...
  QFont Font = this->messageLabel->font();
  QFontMetrics Metrics = QFontMetrics(Font);
  QString Text = this->currentText;
  Text.replace("<br>", " <br> ");
  Text.remove("\n");
  // Counts width of all text...
  int TextWidth = Metrics.width(Text);
  if (TextWidth + 60 <= width) {
    this->messageLabel->setText(this->currentText);
    return;
  }
  // If message is wider than window width...
  this->messageLabel->clear();
  while (TextWidth + 60 > width) {
    QString SingleLine = "";
    if (Text.split(" ").isEmpty()) {
      break;
    }
    QStringList Words = Text.split(" ");
    for (int index = 0; index < Words.length(); index++) {
      // If we have a very large word...
      if (Metrics.width(Words.at(index)) + 60 > width) {
        this->labelAppending(SingleLine);
        Text = Text.remove(0, SingleLine.length()).simplified();
        SingleLine.clear();
        // We eliding it...
        QString Elided =
            Metrics.elidedText(Words.at(index), Qt::ElideRight, width - 80);
        this->labelAppending(Elided);
        Text = Text.remove(0, Words.at(index).length()).trimmed();
        TextWidth = Metrics.width(Text);
        continue;
      }
      if (Metrics.width(SingleLine + Words.at(index)) + 60 >= width)
        break;
      SingleLine += " " + Words.at(index);
      if (Words.at(index) == "<br>")
        break;
    }
    // Cuts the remaining text...
    Text = Text.remove(0, SingleLine.length());
    SingleLine = SingleLine.simplified();
    // Gets new metrics...
    TextWidth = Metrics.width(Text);
    this->labelAppending(SingleLine);
  }
  this->labelAppending(Text);
}

void AkiwakeMessage::commonMaker() {
  // If we have two messages in a row from one user, they are combined into a
  // unit: each previous message looks like a regular board, and only the latter
  // points to the author.
  this->board->LL1->setStyleSheet(
      AkiwakeBoard::borderTheme(this->themeFolder, 6));
  this->board->RL1->setStyleSheet(
      AkiwakeBoard::borderTheme(this->themeFolder, 7));
  this->unitedStyle = true;
}

void AkiwakeMessage::authorDefine() {
  // When changing the topic or adding a message, its authorship is displayed by
  // a special pointer.
  if (this->unitedStyle)
    return;
  if (this->messageAuthor == AkiwakeMessage::ASW)
    this->board->LL1->setStyleSheet(
        AkiwakeBoard::borderTheme(this->themeFolder, 6, true));
  else
    this->board->RL1->setStyleSheet(
        AkiwakeBoard::borderTheme(this->themeFolder, 7, true));
}

void AkiwakeMessage::themeUpdater() {
  // Changes all style sheets.
  this->board->LH->setStyleSheet(
      AkiwakeBoard::borderTheme(this->themeFolder, 1));
  this->board->H->setStyleSheet(
      AkiwakeBoard::borderTheme(this->themeFolder, 2));
  this->board->RH->setStyleSheet(
      AkiwakeBoard::borderTheme(this->themeFolder, 3));
  this->board->L->setStyleSheet(
      AkiwakeBoard::borderTheme(this->themeFolder, 4));
  this->board->centralWidget->setStyleSheet(
      AkiwakeBoard::borderTheme(this->themeFolder, 2));
  this->board->R->setStyleSheet(
      AkiwakeBoard::borderTheme(this->themeFolder, 5));
  this->board->LL1->setStyleSheet(
      AkiwakeBoard::borderTheme(this->themeFolder, 6));
  this->board->L1->setStyleSheet(
      AkiwakeBoard::borderTheme(this->themeFolder, 2));
  this->board->RL1->setStyleSheet(
      AkiwakeBoard::borderTheme(this->themeFolder, 7));
  this->labelTextColor();
  this->authorDefine();
}
