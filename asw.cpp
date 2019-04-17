#include "asw.h"

ASW::ASW(QWidget *parent) : QMainWindow(parent) {
  this->setWindowIcon(QIcon(":/arts/icons/500/icon.png"));
  // Removes interface padding, making it smaller...
  this->layout()->setMargin(0);
  this->layout()->setSpacing(0);
  // Creates central widget and connected widgets...
  QWidget *central = new QWidget();
  central->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
  central->setObjectName("asw_bg");
  central->setStyleSheet("#asw_bg {"
                         "background-image: url(:/arts/background.jpg);"
                         "background-position: center center;"
                         "background-repeat:  no-repeat;"
                         "background-attachment: fixed;"
                         "}");
  QLayout *centralLayout = new QVBoxLayout();
  centralLayout->setSpacing(0);
  this->display = new ADisplay(this);
  this->line = new ALine(this);
  this->mBar = new AMenuBar(this->line);
  // Appends widgets and output...
  centralLayout->addWidget(this->display);
  centralLayout->addWidget(this->line);
  this->setMenuBar(this->mBar);
  central->setLayout(centralLayout);
  this->setCentralWidget(central);
  QSqlDatabase::addDatabase("QSQLITE");
  this->connector();
  this->applyingSettings();
  this->saveSettings();
  emit readyState();
}

void ASW::applyingSettings() {
  this->setWindowTitle(this->ST->application);
  // It's not allowed to reduce the size of the
  // window is less than these values...
  this->setMinimumSize(600, 370);
  this->resize(800, 496);
  // Restores window settings...
  if (this->ST->read(sizeSt).toSize() != QSize(-1, -1))
    this->resize(this->ST->read(sizeSt).toSize());
  if (this->ST->read(isFullscreenSt).toBool()) {
    this->showFullScreen();
    this->mBar->fullScreen->setChecked(true);
    // If ASW starts first time...
    if (!this->ST->read(isNotFirstStartSt).toBool())
      this->fsStarter();
  } else {
    this->saveSettings();
  }
  this->mBar->setVisible(!this->ST->read(isMenubarHiddenSt).toBool());
}

void ASW::connector() {
  // Connects signals with slots.
  connect(this->line->sendButton, &APushButton::clicked, this,
          &ASW::userSendsMessage);
  connect(this->mBar, &AMenuBar::fullscreenModeChanged, this,
          &ASW::fullscreenHandler);
  connect(this->mBar, &AMenuBar::clearScreenTriggered, this, &ASW::clearScreen);
  connect(this->mBar, &AMenuBar::aboutTriggered, this, &ASW::aboutStarter);
  connect(this->mBar, &AMenuBar::contManTriggered, this, &ASW::cmStarter);
  connect(this, &ASW::readyState, this, &ASW::greeting);
}

void ASW::saveSettings() {
  this->ST->write(this->sizeSt, this->size());
  this->ST->write(this->isMenubarHiddenSt, this->mBar->isHidden());
  this->ST->write(this->isFullscreenSt, this->isFullScreen());
  this->ST->write(this->isNotFirstStartSt, true);
}

void ASW::greeting() { this->addMessage(AMessage::ASW, "hello!"); }

void ASW::addMessage(AMessage::A Author, const QString &Text) {
  if (Text.trimmed() == "")
    return;
  AMessage *msg = nullptr;
  if (Author == AMessage::User) {
    this->line->textLine->clear();
    this->display->scrollEnabled = true;
    msg = new AMessage(this);
    msg->setAuthor(Author);
    msg->setMessageType(AMessage::HTML, Text);
  } else {
    msg = TH->get(Text, Author, this);
    if (msg == nullptr)
      return;
    if ((msg->returnMessageType() != AMessage::Widget) and
        (msg->returnText().trimmed() == "")) {
      delete msg;
      return;
    }
  }
  msg->alignTextToWindowWidth(this->width() - 10);
  this->messages.append(msg);
  this->display->layout->addWidget(msg);
  // Responses to user expression...
  if (Author == AMessage::User)
    this->addMessage(AMessage::ASW, Text);
}

void ASW::userSendsMessage() {
  this->addMessage(AMessage::User, this->line->textLine->text());
}

void ASW::resizeEvent(QResizeEvent *event) {
  foreach (AMessage *msg, this->messages)
    msg->alignTextToWindowWidth(this->width() - 10);
  event->accept();
}

void ASW::keyPressEvent(QKeyEvent *event) {
  if ((event->modifiers() == Qt::ControlModifier) &&
      (event->key() == Qt::Key_H))
    this->mBar->setVisible(!this->mBar->isVisible());
  if (!this->mBar->isVisible()) {
    if (event->key() == Qt::Key_F11) {
      this->mBar->fullScreen->setChecked(!this->mBar->fullScreen->isChecked());
      this->fullscreenHandler();
    }
    if ((event->modifiers() == (Qt::ControlModifier | Qt::AltModifier)) &&
        (event->key() == Qt::Key_D))
      this->clearScreen();
  }
}

void ASW::fullscreenHandler() {
  if (this->mBar->fullScreen->isChecked())
    this->showFullScreen();
  else
    this->showNormal();
}

void ASW::clearScreen() {
  this->display->start();
  this->messages.clear();
}

void ASW::fsStarter() { this->addMessage(AMessage::User, "/first"); }

void ASW::aboutStarter() { this->addMessage(AMessage::User, "/about"); }

void ASW::cmStarter() { this->addMessage(AMessage::User, "/cm"); }

void ASW::helpStarter() { this->addMessage(AMessage::User, "/help"); }
