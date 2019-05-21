#include "about.h"

/*
 * All short named objects and their explanations:
 * {objn} <- object name
 * {fw} <- fixed width
 * {fh} <- fixed height
 * {lt} <- layout
 * {ap} <- ASW picture
 * {at} <- ASW title
 * {sp} <- spacer
 * {ts} <- tabs
 * {sa1}, {sa2} <- scroll areas
 */

/*!
 * Argument: QWidget {*parent}.
 * Customizes the dialog box.
 *
 * Layout scheme:
 * <-------------->
 * [Logo][App name]
 * [  Tab widget  ]
 * {About}{Authors}
 * [<->][Close btn]
 * <-------------->
 */
About::About(QWidget *parent) : QWidget(parent) {
  setAttribute(Qt::WA_DeleteOnClose);
  setObjectName(objn);
  setFixedSize(fw, fh);
  auto *lt = new QGridLayout();
  lt->setMargin(0);
  lt->setSpacing(0);
  auto *ap = new QLabel("<img src=\":/arts/littleasw.png\">", this);
  auto *at = new QLabel(this);
  at->setText("<font size=\"4\">" + QTranslator::tr("Associative system") +
              "</font>");
  at->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  auto *sp = new QSpacerItem(0, 0, QSizePolicy::Preferred, QSizePolicy::Fixed);
  auto *ts = new QTabWidget(this);
  auto *tab1 = new QLabel(this);
  tab1->setText(QTranslator::tr("ASW - Associative System Window") +
                "<br><br>© 2018-2019 " + QTranslator::tr("The ASW Authors") +
                "<br><br><a href=\"https://github.com/kollieartwolf/asw\">" +
                QTranslator::tr("ASW on GitHub") + "</a>");
  tab1->setOpenExternalLinks(true);
  tab1->setAlignment(Qt::AlignTop | Qt::AlignLeft);
  auto *tab2 = new QLabel(this);
  tab2->setText(
      "<b>" + QTranslator::tr("Shamshin Victor") + "</b><br>" +
      QTranslator::tr("Developer") +
      "<br><a "
      "href=\"mailto:androiddevelop@yandex.ru\">androiddevelop@yandex.ru</"
      "a><br><br><b>" +
      QTranslator::tr("Titov Klimenty") + "</b><br>" +
      QTranslator::tr("Developer") +
      "<br><a href=\"mailto:aclo.create@gmail.com\">aclo.create@gmail.com</a>");
  tab2->setOpenExternalLinks(true);
  tab2->setAlignment(Qt::AlignTop | Qt::AlignLeft);
  auto *sa1 = new QScrollArea(this);
  auto *sa2 = new QScrollArea(this);
  sa1->setFocusPolicy(Qt::NoFocus);
  sa1->setFrameStyle(QFrame::NoFrame);
  sa1->setFrameShadow(QFrame::Plain);
  sa2->setFocusPolicy(Qt::NoFocus);
  sa2->setFrameStyle(QFrame::NoFrame);
  sa2->setFrameShadow(QFrame::Plain);
  auto *bottomLine = new QWidget(this);
  auto *closeBtn = new AButton(QTranslator::tr("Close"), bottomLine);
  auto *bottomLayout = new QHBoxLayout();
  bottomLayout->setMargin(0);
  bottomLayout->setSpacing(0);
  auto *bottomSpacer =
      new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Fixed);
  bottomLayout->addItem(bottomSpacer);
  bottomLayout->addWidget(closeBtn);
  bottomLine->setLayout(bottomLayout);
  connect(closeBtn, &AButton::clicked, this, &QWidget::close);
  sa1->setWidget(tab1);
  sa2->setWidget(tab2);
  ts->addTab(sa1, QTranslator::tr("About"));
  ts->addTab(sa2, QTranslator::tr("Authors"));
  lt->addWidget(ap, 0, 0);
  lt->addWidget(at, 0, 1);
  lt->addItem(sp, 0, 2);
  lt->addWidget(ts, 1, 0, 1, 3);
  lt->addWidget(bottomLine, 2, 0, 1, 3);
  setLayout(lt);
}
