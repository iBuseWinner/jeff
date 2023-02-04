#include "help.hpp"

/*! @brief The constructor. */
Help::Help(QWidget *parent, ModalHandler *mhandler) : Dialog(mhandler, parent) {
  setFixedHeight(360);
  setFixedWidth(400);
  title = new QLabel(this);
  text = new QLabel(this);
  text->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::LinksAccessibleByMouse);
  auto *prev_btn = new Button(tr("Previous tip"), this);
  auto *next_btn = new Button(tr("Next tip"), this);
  auto *close_btn = new Button(tr("Close"), this);
  close_btn->setIcon(QIcon::fromTheme("collapse-all", QIcon(":/arts/icons/16/collapse-all.svg")));
  connect(close_btn, &Button::clicked, this, &Dialog::close);
  auto *area_widget = new QWidget(this);
  area_widget->setContentsMargins(0, 0, 5, 0);
  area_widget->setLayout(VLineLt::another()->addw(title)->addw(text));
  auto *scroll_area = new ScrollArea(this);
  scroll_area->setWidget(area_widget);
  setLayout(GridLt::another()
    ->addw(scroll_area)
    ->addlt(HLineLt::another()->addw(prev_btn)->addw(next_btn)->addw(close_btn))
  );
  _set_tip();
}

/*! @brief TBD */
void Help::set_tip(HelpTipListDirection direction) {
  
}

/*! @brief TBD */
void Help::_set_tip(int tip_number) {
  
}
