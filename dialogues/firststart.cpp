#include "firststart.h"

/*!
 * @brief The constructor.
 * @details Layout scheme:
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
  setObjectName(object_name);
  setFixedWidth(fixed_width);
  auto *layout = new QVBoxLayout();
  auto *title = new QLabel("<font size=\"6\">" + tr("Jeff") + "</font>", this);
  auto *startText = new QLabel(
    "<font size=\"3\">" + tr("This is the first start of Jeff. Before you start working:") +
    "<br>" + tr("1) go to the Source Manager...") + "<p><img src=\":/arts/shots/menufile.png\"></p>" +
    tr("2) add databases...") + "<p><img src=\":/arts/shots/addsource.png\"></p>" + 
    tr("And now you've done.") + "</font>", this
  );
  startText->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::LinksAccessibleByMouse);
  auto *bottomLine = new QWidget(this);
  auto *closeBtn = new Button(tr("Close"), bottomLine);
  auto *bottomLayout = new QHBoxLayout();
  bottomLayout->setMargin(0);
  bottomLayout->setSpacing(0);
  auto *spacer = new QSpacerItem(0, 0, QSizePolicy::Preferred, QSizePolicy::Fixed);
  bottomLayout->addItem(spacer);
  bottomLayout->addWidget(closeBtn);
  bottomLine->setLayout(bottomLayout);
  connect(closeBtn, &Button::clicked, this, [this] { _m_handler->closePrisoner(); });
  layout->addWidget(title);
  layout->addWidget(startText);
  layout->addWidget(bottomLine);
  setLayout(layout);
}
