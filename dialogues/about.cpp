#include "about.h"

About::About(QWidget *parent) : AkiwakeDialog(parent) {
  this->setWindowTitle("About ASW");
  // Creating main objects...
  auto *entireLayout = new QGridLayout();
  auto *aswpic = new QLabel("<img src=\":/arts/littleasw.png\">", this);
  auto *aswtitle = new QLabel("<font size=\"4\">Associative system</font>");
  aswtitle->setAlignment(Qt::AlignTop | Qt::AlignLeft);
  auto *inlineSpacer =
      new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Fixed);
  auto *tabs = new QTabWidget(this);
  auto *aboutTab = new QLabel(
      "ASW - Associative System Window<br><br>(c) 2018-2019 The ASW "
      "Authors<br><br><a href=\"https://github.com/kollieartwolf/asw\">ASW on "
      "GitHub</a>",
      this);
  aboutTab->setOpenExternalLinks(true);
  aboutTab->setAlignment(Qt::AlignTop | Qt::AlignLeft);
  auto *authorsTab = new QLabel(
      "<b>Shamshin Victor</b><br>Developer<br><a "
      "href=\"mailto:androiddevelop@yandex.ru\">androiddevelop@yandex.ru</"
      "a><br><br><b>Titov Klimenty</b><br>Developer<br><a "
      "href=\"mailto:aclo.create@gmail.com\">aclo.create@gmail.com</a>",
      this);
  authorsTab->setOpenExternalLinks(true);
  authorsTab->setAlignment(Qt::AlignTop | Qt::AlignLeft);
  auto *aboutSc = new QScrollArea(this);
  auto *authorsSc = new QScrollArea(this);
  aboutSc->setFocusPolicy(Qt::NoFocus);
  aboutSc->setFrameStyle(QFrame::NoFrame);
  aboutSc->setFrameShadow(QFrame::Plain);
  authorsSc->setFocusPolicy(Qt::NoFocus);
  authorsSc->setFrameStyle(QFrame::NoFrame);
  authorsSc->setFrameShadow(QFrame::Plain);
  aboutSc->setWidget(aboutTab);
  authorsSc->setWidget(authorsTab);
  tabs->addTab(aboutSc, "About");
  tabs->addTab(authorsSc, "Authors");
  entireLayout->addWidget(aswpic, 0, 0);
  entireLayout->addWidget(aswtitle, 0, 1);
  entireLayout->addItem(inlineSpacer, 0, 2);
  entireLayout->addWidget(tabs, 1, 0, 1, 3);
  this->mainBoard->centralWidget->setLayout(entireLayout);
  this->applyingSettings();
}

void About::applyingSettings() {
  auto *ST = new SettingsStore();
  this->resize(320, 240);
  if (ST->read(sizeSt).toSize() != QSize(-1, -1))
    this->resize(ST->read(sizeSt).toSize());
  delete ST;
}

About::~About() {
  auto *ST = new SettingsStore();
  ST->write(this->sizeSt, this->size());
  delete ST;
}
