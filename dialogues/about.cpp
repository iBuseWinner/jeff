#include "about.h"

About::About(QWidget *p) : QWidget(p) {
  setAttribute(Qt::WA_DeleteOnClose);
  setObjectName("about");
  setFixedSize(fw, fh);
  auto *el = new QGridLayout();
  el->setMargin(0);
  el->setSpacing(0);
  auto *ap = new QLabel("<img src=\":/arts/littleasw.png\">", this);
  auto *at = new QLabel("<font size=\"4\">Associative system</font>");
  at->setAlignment(Qt::AlignTop | Qt::AlignLeft);
  auto *is = new QSpacerItem(0, 0, QSizePolicy::Preferred, QSizePolicy::Fixed);
  auto *ts = new QTabWidget(this);
  auto *aTab = new QLabel(
      "ASW - Associative System Window<br><br>(c) 2018-2019 The ASW "
      "Authors<br><br><a href=\"https://github.com/kollieartwolf/asw\">ASW on "
      "GitHub</a>",
      this);
  aTab->setOpenExternalLinks(true);
  aTab->setAlignment(Qt::AlignTop | Qt::AlignLeft);
  auto *asTab = new QLabel(
      "<b>Shamshin Victor</b><br>Developer<br><a "
      "href=\"mailto:androiddevelop@yandex.ru\">androiddevelop@yandex.ru</"
      "a><br><br><b>Titov Klimenty</b><br>Developer<br><a "
      "href=\"mailto:aclo.create@gmail.com\">aclo.create@gmail.com</a>",
      this);
  asTab->setOpenExternalLinks(true);
  asTab->setAlignment(Qt::AlignTop | Qt::AlignLeft);
  auto *aboutSc = new QScrollArea(this);
  auto *authorsSc = new QScrollArea(this);
  aboutSc->setFocusPolicy(Qt::NoFocus);
  aboutSc->setFrameStyle(QFrame::NoFrame);
  aboutSc->setFrameShadow(QFrame::Plain);
  authorsSc->setFocusPolicy(Qt::NoFocus);
  authorsSc->setFrameStyle(QFrame::NoFrame);
  authorsSc->setFrameShadow(QFrame::Plain);
  auto *okLine = new QWidget(this);
  auto *ok = new AButton("Close", okLine);
  auto *okLayout = new QHBoxLayout();
  okLayout->setMargin(0);
  okLayout->setSpacing(0);
  auto *okSpacer =
      new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Fixed);
  okLayout->addItem(okSpacer);
  okLayout->addWidget(ok);
  okLine->setLayout(okLayout);
  connect(ok, &AButton::clicked, this, &QWidget::close);
  aboutSc->setWidget(aTab);
  authorsSc->setWidget(asTab);
  ts->addTab(aboutSc, "About");
  ts->addTab(authorsSc, "Authors");
  el->addWidget(ap, 0, 0);
  el->addWidget(at, 0, 1);
  el->addItem(is, 0, 2);
  el->addWidget(ts, 1, 0, 1, 3);
  el->addWidget(okLine, 2, 0, 1, 3);
  setLayout(el);
}
