#include "firststart.h"

/*
 * All short named objects and their explanations:
 * {objn} <- object name
 */

/*!
 * Argument: QWidget {*parent}.
 * Constructs and prepares First Start.
 *
 * Layout scheme:
 * <-------------->
 * [    Title     ]
 * [  Start text  ]
 * [ Bottom  line ]
 * {<->}{Close btn}
 * <-------------->
 */
FirstStart::FirstStart(QWidget *parent, ModalHandler *m_handler)
    : QWidget(parent), _m_handler(m_handler) {
  _m_handler->setPrisoner(this);
  setObjectName(objn);
  auto *layout = new QVBoxLayout();
  auto *title = new QLabel(
      "<font size=\"6\">" + QTranslator::tr("Associative system") + "</font>",
      this);
  auto *startText = new QLabel(
      "<font size=\"3\">" +
          QTranslator::tr(
              "This is the first start of ASW. Before you start working:") +
          "<br>" + QTranslator::tr("1) go to the Source Manager...") +
          "<p><img src=\":/arts/shots/menufile.png\"></p>" +
          QTranslator::tr("2) add databases...") +
          "<p><img src=\":/arts/shots/addsource.png\"></p>" +
          QTranslator::tr("And now you've done.") + "</font>",
      this);
  startText->setTextInteractionFlags(Qt::TextSelectableByMouse |
                                     Qt::LinksAccessibleByMouse);
  auto *bottomLine = new QWidget(this);
  auto *closeBtn = new AButton(QTranslator::tr("Close"), bottomLine);
  auto *bottomLayout = new QHBoxLayout();
  bottomLayout->setMargin(0);
  bottomLayout->setSpacing(0);
  auto *spacer =
      new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Fixed);
  bottomLayout->addItem(spacer);
  bottomLayout->addWidget(closeBtn);
  bottomLine->setLayout(bottomLayout);
  connect(closeBtn, &AButton::clicked, this,
          [this] { _m_handler->closePrisoner(); });
  layout->addWidget(title);
  layout->addWidget(startText);
  layout->addWidget(bottomLine);
  setLayout(layout);
}
