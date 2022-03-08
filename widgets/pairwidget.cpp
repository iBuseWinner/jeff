#include "pairwidget.h"

/*! @brief The constructor. */
PairWidget::PairWidget(QWidget *w1, QWidget *w2, QWidget *parent) : QWidget(parent) {
  w1->setParent(this);
  w2->setParent(this);
  auto *lt = new QHBoxLayout();
  lt->setSpacing(0);
  lt->setMargin(0);
  lt->addWidget(w1);
  lt->addWidget(w2);
  setLayout(lt);
}
