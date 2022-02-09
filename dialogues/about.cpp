#include "about.h"

/*!
 * @brief The constructor.
 * @details Layout scheme:
 * <-------------->
 * [Logo][App name]
 * [  Tab widget  ]
 * {About}{Authors}
 * {<->}[Close btn]
 * <-------------->
 */
About::About(QWidget *parent, ModalHandler *m_handler) : ScrollFreezerWidget(parent), _m_handler(m_handler) {
  _m_handler->setPrisoner(this);
  setObjectName(object_name);
  setFixedWidth(fixed_width);
  auto *layout = new QGridLayout();
  layout->setMargin(0);
  layout->setSpacing(0);
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
    tr("Jeff - chat bot and automatization tool") + "<br><br>© 2018-2022 " +
    tr("Jeff Authors") + "<br><br><a href=\"https://github.com/markcda/cc-jeff\">"
    "https://github.com/markcda/cc-jeff</a><br><a href=\"https://raw.githubusercontent"
    ".com/markcda/cc-jeff/master/LICENSE\">" + tr("License") +
    ": GNU General Public License, " + tr("version") + " 3</a>"
  );
  tab1->setOpenExternalLinks(true);
  tab1->setAlignment(Qt::AlignTop | Qt::AlignLeft);
  auto *tab2 = new QLabel(this);
  tab2->setText(
    "<b>" + tr("Shamshin Victor") + "</b><br>" + tr("Developer") +
    "<br><a href=\"mailto:androiddevelop@yandex.ru\">androiddevelop@yandex.ru</a><br><br><b>" +
    tr("Mark CDA") + "</b><br>" + tr("Developer") + "<br><a "
    "href=\"mailto:markcda@protonmail.com\">markcda@protonmail.com</a>"
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
  auto *bottomLine = new QWidget(this);
  auto *closeBtn = new Button(tr("Close"), bottomLine);
  auto *bottomLayout = new QHBoxLayout();
  bottomLayout->setMargin(0);
  bottomLayout->setSpacing(0);
  auto *bottomSpacer = new QSpacerItem(0, 0, QSizePolicy::Preferred, QSizePolicy::Fixed);
  bottomLayout->addItem(bottomSpacer);
  bottomLayout->addWidget(closeBtn);
  bottomLine->setLayout(bottomLayout);
  connect(closeBtn, &Button::clicked, this, [this] { _m_handler->closePrisoner(); });
  scroll_area_1->setWidget(tab1);
  scroll_area_2->setWidget(tab2);
  tabs->addTab(scroll_area_1, tr("About"));
  tabs->addTab(scroll_area_2, tr("Authors"));
  layout->addWidget(logo, 0, 0, 2, 1);
  layout->addWidget(title, 0, 1);
  layout->addWidget(version, 1, 1);
  layout->addItem(spacer, 0, 2, 2, 1);
  layout->addWidget(tabs, 2, 0, 1, 3);
  layout->addWidget(bottomLine, 3, 0, 1, 3);
  setLayout(layout);
}
