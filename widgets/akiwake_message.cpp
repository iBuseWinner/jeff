#include "akiwake_message.h"

AkiwakeMessage::AkiwakeMessage(const QString &Text, AuthorType Author,
                               ThemeType Theme = AkiwakeMessage::Light,
                               QWidget *parent)
    : QWidget(parent) {
  // Creates a message.
  // Variable assignment...
  this->themeFolder = Theme;
  this->messageAuthor = Author;
  this->messageLabel = new QLabel(Text, this);
  this->messageLabel->setTextFormat(Qt::RichText);
  this->currentText = Text;
  this->messageLabel->setTextInteractionFlags(Qt::TextSelectableByMouse |
                                              Qt::LinksAccessibleByMouse);
  this->messageLabel->setFocusPolicy(Qt::NoFocus);
  this->messageLabel->setOpenExternalLinks(true);
  this->labelTextColor();
  this->themeUpdater();
  // Creating layout for adding message into AkiwakeBoard...
  QLayout *labelLayout = new QGridLayout();
  labelLayout->setMargin(3);
  labelLayout->setSpacing(0);
  labelLayout->addWidget(this->messageLabel);
  this->board->centralWidget->setLayout(labelLayout);
  // Creating message output format...
  auto *line = new QHBoxLayout(this);
  line->setContentsMargins(0, 0, 0, 6);
  line->setSpacing(0);
  auto *lineSpacer =
      new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Fixed);
  if (this->messageAuthor == AkiwakeMessage::ASW) {
    line->addWidget(this->board);
    line->addSpacerItem(lineSpacer);
  } else {
    line->addSpacerItem(lineSpacer);
    line->addWidget(this->board);
  }
  this->setLayout(line);
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

void AkiwakeMessage::labelAppending(const QString &Text) {
  // Transfer part of the text to a new line.
  this->messageLabel->setText(this->messageLabel->text() + "\n" +
                              Text.simplified());
}

void AkiwakeMessage::textLayoutDesigner(int width) {
  // Makes messages more readable no matter what text is written.
  // Takes metrics from message label font...
  // WARNING!!! THIS IS NOW REFORMING!

  QFont Font = this->messageLabel->font();
  QFontMetrics Metrics = QFontMetrics(Font);
  QString Text = this->currentText;
  Text.remove("\n");
  Text.replace("<br>", " <br> ");
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
    for (QString Word : Words) {
      // If we have a very large word...
      if (Metrics.width(Word) + 60 > width) {
        this->labelAppending(SingleLine);
        Text = Text.remove(0, SingleLine.length()).simplified();
        SingleLine.clear();
        // We eliding it...
        QString Elided = Metrics.elidedText(Word, Qt::ElideRight, width - 80);
        this->labelAppending(Elided);
        Text = Text.remove(0, Word.length()).trimmed();
        TextWidth = Metrics.width(Text);
        continue;
      }
      if (Metrics.width(QString(SingleLine + " " + Word).replace(" <br>", "")) +
              60 >=
          width)
        break;
      SingleLine += " " + Word;
      if (Word == "<br>")
        break;
    }
    // Cuts the remaining text...
    Text = Text.remove(0, SingleLine.length());
    SingleLine.remove("<br>");
    SingleLine = SingleLine.simplified();
    // Gets new metrics...
    TextWidth = Metrics.width(Text);
    this->labelAppending(SingleLine);
  }
  this->labelAppending(Text);
  this->messageLabel->setText(this->messageLabel->text().trimmed());
  this->messageLabel->setText(this->messageLabel->text().replace("\n", "<br>"));
}

void AkiwakeMessage::themeUpdater() { this->labelTextColor(); }
