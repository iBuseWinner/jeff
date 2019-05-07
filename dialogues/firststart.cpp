#include "firststart.h"

FirstStart::FirstStart(QWidget *p) : QWidget(p) {
  setAttribute(Qt::WA_DeleteOnClose);
  setObjectName("firststart");
  setFixedWidth(400);
  auto *el = new QVBoxLayout();
  auto *title = new QLabel("<font size=\"6\">Associative System</font>", this);
  auto *startText = new QLabel(
      "<font size=\"3\">This is the first start of "
      "ASW. Before you start working:<br>"
      "1) go to the Container Manager..."
      "<p><img src=\":/arts/shots/menufile.png\"></p>"
      "2) add databases..."
      "<p><img src=\":/arts/shots/addcontainer.png\"></p>"
      "And now you've done.</font>",
      this);
  startText->setTextInteractionFlags(Qt::TextSelectableByMouse |
                                     Qt::LinksAccessibleByMouse);
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
  el->addWidget(title);
  el->addWidget(startText);
  el->addWidget(okLine);
  setLayout(el);
}
