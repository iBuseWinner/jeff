#include "firststart.hpp"

/*! @brief The constructor.
 *  @details Layout scheme:
 *  <-------------->
 *  [    Title     ]
 *  [  Start text  ]
 *  [ Bottom  line ]
 *  {<->}{Close btn}
 *  <-------------->  */
FirstStart::FirstStart(QWidget *parent, ModalHandler *mhandler) : Dialog(mhandler, parent) {
  setFixedHeight(360);
  setFixedWidth(400);
  auto *title = new QLabel("<h1><b>" + tr("Jeff") + "</b></h1>", this);
  auto *startText = new QLabel(
    "<font size=\"3\">" + tr("This is the first start of Jeff. Before you start working:") +
    "<br>" + tr("1) go to the Source Manager...") + "<p><img src=\":/arts/shots/menufile.png\"></p>" +
    tr("2) add databases...") + "<p><img src=\":/arts/shots/addsource.png\"></p>" + 
    tr("And now you've done.") + "</font>", this
  );
  startText->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::LinksAccessibleByMouse);
  auto *footer_line = new QWidget(this);
  auto *close_btn = new Button(tr("Close"), footer_line);
  close_btn->setIcon(
    QIcon::fromTheme("collapse-all", QIcon(":/arts/icons/16/collapse-all.svg")));
  footer_line->setLayout(HLineLt::another()
    ->spacing()
    ->addi(new QSpacerItem(0, 0, QSizePolicy::Preferred, QSizePolicy::Fixed))
    ->addw(close_btn)
  );
  connect(close_btn, &Button::clicked, this, &Dialog::close);
  auto *area_widget = new QWidget(this);
  area_widget->setContentsMargins(0, 0, 5, 0);
  area_widget->setLayout(VLineLt::another()->addw(title)->addw(startText)->addw(footer_line));
  auto *scroll_area = new ScrollArea(this);
  scroll_area->setWidget(area_widget);
  setLayout(GridLt::another()->addw(scroll_area));
}
