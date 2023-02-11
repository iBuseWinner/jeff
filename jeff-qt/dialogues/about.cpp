#include "about.hpp"

/*! @brief The constructor.
 *  @details Layout scheme:
 *  <-------------->
 *  [Logo][App name]
 *  [  Tab widget  ]
 *  {About}{Authors}
 *  {<->}[Close btn]
 *  <-------------->  */
About::About(QWidget *parent, ModalHandler *mhandler) : ScrollFreezerWidget(parent) {
  mhandler->setPrisoner(this);
  setFixedWidth(fixed_width);
  auto *logo = new QLabel(this);
  logo->setScaledContents(true);
  logo->setFixedWidth(fixed_width / 2);
  logo->setPixmap(QPixmap(":/arts/jeff.png").scaledToWidth(logo->width(), Qt::SmoothTransformation));
  auto *title = new QLabel(this);
  title->setText("<font size=\"4\">" + tr("Jeff") + "</font>");
  title->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  auto *version = new QLabel(this);
  QString version_title = tr("version");
  version_title[0] = version_title[0].toUpper();
  version->setText(version_title + " " + QCoreApplication::applicationVersion());
  version->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  auto *spacer = new QSpacerItem(0, 0, QSizePolicy::Preferred, QSizePolicy::Fixed);
  auto *tabs = new QTabWidget(this);
  tabs->setStyleSheet(
    QString("background-color: %1; color: %2;").arg(styling.css_bg_color).arg(styling.css_fg_color)
  );
  auto *tab1 = new QLabel(this);
  tab1->setText(
    tr("Jeff - chatbot and automation tool") + "<br><br>© 2018-2023 " +
    tr("Jeff Authors") + "<br><br><a href=\"https://github.com/markcda/jeff\">"
    "https://github.com/markcda/jeff</a><br>" + tr("License") + ": <a href=\"https://raw.githubusercontent.com/markcda/jeff/master/LICENSE\">MIT License"
  );
  tab1->setOpenExternalLinks(true);
  tab1->setAlignment(Qt::AlignTop | Qt::AlignLeft);
  auto *tab2 = new QLabel(this);
  tab2->setText(
    "<b>" + tr("Shamshin Victor") + "</b><br>" + tr("Developer") +
    "<br><a href=\"mailto:androiddevelop@yandex.ru\">androiddevelop@yandex.ru</a><br><br><b>" +
    tr("Mark CDA") + "</b><br>" + tr("Developer") + "<br><a "
    "href=\"mailto:creativecorp.markcda@proton.me\">creativecorp.markcda@proton.me</a>"
  );
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
  auto *footer_w = new QWidget(this);
  auto *close_btn = new Button(tr("Close"), footer_w);
  close_btn->setIcon(
    QIcon::fromTheme("collapse-all", QIcon(":/arts/icons/16/collapse-all.svg")));
  footer_w->setLayout(HLineLt::another()
    ->spacing()->addi(new QSpacerItem(0, 0, QSizePolicy::Preferred, QSizePolicy::Fixed))->addw(close_btn)
  );
  connect(close_btn, &Button::clicked, this, [this, mhandler] { mhandler->closePrisoner(); });
  scroll_area_1->setWidget(tab1);
  scroll_area_2->setWidget(tab2);
  tabs->addTab(scroll_area_1, tr("About"));
  tabs->addTab(scroll_area_2, tr("Authors"));
  setLayout(GridLt::another()
    ->spacing(3)->addw(logo, 0, 0, 2, 1)->addw(title, 0, 1)->addw(version, 1, 1)
    ->addi(spacer, 0, 2, 2, 1)->addw(tabs, 2, 0, 1, 3)->addw(footer_w, 3, 0, 1, 3)
  );
}
