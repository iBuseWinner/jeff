#include "asw.h"
#include <QDebug>

ASW::ASW(QWidget *parent) : QMainWindow(parent) {
  SettingsStore *ST = new SettingsStore;
  this->setWindowTitle(ST->application); // windowsTitle = "ASW"
  this->setMinimumSize(300, 150); // It's not allowed to reduce the size of the
                                  // window is less than these values.
  // Restores window settings...
  if (ST->read("asw/size").toSize() !=
      QSize(-1, -1)) // if record of ASW window size exist
    this->resize(ST->read("asw/size").toSize());
  else
    this->resize(800, 496); // default size
  delete ST;
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
  this->display = new AkiwakeDisplay();
  this->line = new AkiwakeLine();
  this->mBar = new AkiwakeMenuBar();
  // Appends widgets and output...
  central->setLayout(centralLayout);
  central->layout()->addWidget(this->display);
  central->layout()->addWidget(this->line);
  this->setMenuBar(mBar);
  this->setCentralWidget(central);
  QSqlDatabase::addDatabase("QSQLITE");

  /* sqlite sq;
   * sq.create_db("tuta.db", "elastica");
   * sq.insert("tuta.db", "elastica", 0, "Hello!", "1");
   * sq.insert("tuta.db", "elastica", 1, "Hi!", "0"); */

  // /*
  // QList<containerProperties> CP;
  // containerProperties CPa;
  // CPa.path = "hello";
  // CPa.container = "it's me";
  // CP.append(CPa);
  // CP.append(CPa);
  // sq.writeSelection(CP);
  // */

  // Creates a welcome message...
  thinking *TH = new thinking;
  TH->putExpression("Hello!");
  TH->startProcessing();
  this->addMessage(AkiwakeMessage::ASW, TH->get());
  delete TH;

  this->connector();
}

void ASW::connector() {
  // Connects signals with slots.
  connect(this->line->sendButton, &AkiwakePushButton::clicked, this,
          &ASW::userSendsMessage);
}

ASW::~ASW() {}

void ASW::resizeEvent(QResizeEvent *event) {
  // Calibrates all messages.
  for (int i = 0; i < this->messages.length(); i++)
    messages.at(i)->textLayoutDesigner(this->width());
  event->accept();
}

void ASW::closeEvent(QCloseEvent *event) {
  // Saves the window settings.
  SettingsStore *ST = new SettingsStore();
  ST->write("asw/size", this->size());
  delete ST;
  event->accept();
}

void ASW::addMessage(AkiwakeMessage::AuthorType Author, QString Text = "") {
  // Text preparation...
  if (Author == AkiwakeMessage::User) {
    Text = this->line->textLine->text();
    this->line->textLine->clear();
    this->display->scrollEnabled = true;
  }
  if (Text.trimmed() == "")
    return;
  AkiwakeMessage *msg = new AkiwakeMessage(Text, Author, this->themeFolder);
  // Style unification...
  if ((this->current != nullptr) &&
      (this->current->messageAuthor == msg->messageAuthor))
    this->current->commonMaker();
  // Calibrates the message and add it to the screen...
  msg->textLayoutDesigner(this->width());
  this->current = msg;
  this->messages.append(msg);
  this->display->layout->addWidget(this->current);
  // Responses to user expression...
  if (Author == AkiwakeMessage::User) {
    thinking *TH = new thinking;
    TH->putExpression(Text);
    TH->startProcessing();
    this->addMessage(AkiwakeMessage::ASW, TH->get());
    delete TH;
  }
}

void ASW::userSendsMessage() {
  // Redirecting...
  this->addMessage(AkiwakeMessage::User);
}

void ASW::themeUpdater() {
  // Changes message color.
  for (int i = 0; i < this->messages.length(); i++) {
    messages.at(i)->themeFolder = this->themeFolder;
    messages.at(i)->themeUpdater();
  }
}
