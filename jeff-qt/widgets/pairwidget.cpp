#include "pairwidget.h"

/*! @brief The constructor. */
PairWidget::PairWidget(QWidget *w1, QWidget *w2, QWidget *parent) : QWidget(parent) {
  w1->setParent(this);
  w2->setParent(this);
  setLayout(HLineLt::another()->spacing()->addw(w1)->addw(w2));
}
