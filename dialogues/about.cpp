#include "about.h"

/*
 * All short named objects and their explanations:
 * {objn} <- object name
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
  setFixedSize(fixed_width, fixed_height);
  auto *layout = new QGridLayout();
  layout->setMargin(0);
  layout->setSpacing(0);
  auto *logo = new QLabel("<img src=\":/arts/littleasw.png\">", this);
  auto *title = new QLabel(this);
  title->setText("<font size=\"4\">" + QTranslator::tr("Associative system") +
                 "</font>");
  title->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  auto *spacer =
      new QSpacerItem(0, 0, QSizePolicy::Preferred, QSizePolicy::Fixed);
  auto *tabs = new QTabWidget(this);
  auto *tab1 = new QLabel(this);
  tab1->setText(QTranslator::tr("ASW - Associative System Window") +
                "<br><br>© 2018-2020 " + QTranslator::tr("The ASW Authors") +
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
  auto *scroll_area_1 = new QScrollArea(this);
  auto *scroll_area_2 = new QScrollArea(this);
  scroll_area_1->setFocusPolicy(Qt::NoFocus);
  scroll_area_1->setFrameStyle(QFrame::NoFrame);
  scroll_area_1->setFrameShadow(QFrame::Plain);
  scroll_area_2->setFocusPolicy(Qt::NoFocus);
  scroll_area_2->setFrameStyle(QFrame::NoFrame);
  scroll_area_2->setFrameShadow(QFrame::Plain);
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
  scroll_area_1->setWidget(tab1);
  scroll_area_2->setWidget(tab2);
  tabs->addTab(scroll_area_1, QTranslator::tr("About"));
  tabs->addTab(scroll_area_2, QTranslator::tr("Authors"));
  layout->addWidget(logo, 0, 0);
  layout->addWidget(title, 0, 1);
  layout->addItem(spacer, 0, 2);
  layout->addWidget(tabs, 1, 0, 1, 3);
  layout->addWidget(bottomLine, 2, 0, 1, 3);
  setLayout(layout);
}
