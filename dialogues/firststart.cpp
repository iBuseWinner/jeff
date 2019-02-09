#include "firststart.h"

FirstStart::FirstStart(QWidget *parent) : AkiwakeDialog(parent) {
  this->setWindowTitle("First start");
  // Creates main objects...
  auto *scroller = new QScrollArea(this);
  scroller->setFocusPolicy(Qt::NoFocus);
  scroller->setFrameStyle(QFrame::NoFrame);
  scroller->setFrameShadow(QFrame::Plain);
  auto *elL = new QGridLayout();
  auto *entireWidget = new QWidget(this);
  auto *entireLayout = new QVBoxLayout();
  auto *title = new QLabel("<font size=\"8\">Associative System</font>", this);
  auto *startText = new QLabel("<font size=\"4\">This is the first start of "
                               "ASW. Before you start working:<br>"
                               "1) go to the Container Manager..."
                               "<p><img src=\":/arts/shots/menufile.png\"></p>"
                               "2) add databases..."
                               "<p><img src=\":/arts/shots/addcontainer.png\"></p>"
                               "And now you've done.</font>",
                               this);
  startText->setTextInteractionFlags(Qt::TextSelectableByMouse |
                                     Qt::LinksAccessibleByMouse);
  auto *lineSpacer =
      new QSpacerItem(0, 0, QSizePolicy::Fixed, QSizePolicy::Expanding);
  entireLayout->addWidget(title);
  entireLayout->addWidget(startText);
  entireLayout->addItem(lineSpacer);
  entireWidget->setLayout(entireLayout);
  scroller->setWidget(entireWidget);
  elL->addWidget(scroller);
  this->mainBoard->centralWidget->setLayout(elL);
}
