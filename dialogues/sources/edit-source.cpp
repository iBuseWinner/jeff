#include "edit-source.h"

EditSourceDialog::EditSourceDialog(Basis *_basis, QWidget *parent)
    : QWidget(parent), basis(_basis) {
  auto *layout = new QGridLayout();
  activators = new AExpressionList(this);
  curr = new AExpressionList(this);
  reagents = new AExpressionList(this);
  auto *activators_label = new QLabel(tr("Activators"), this);
  auto *curr_label = new QLabel(tr("Expressions"), this);
  auto *reagents_label = new QLabel(tr("Reagents"), this);
  layout->addWidget(activators_label, 0, 0);
  layout->addWidget(curr_label, 0, 1);
  layout->addWidget(reagents_label, 0, 2);
  layout->addWidget(activators, 1, 0);
  layout->addWidget(curr, 1, 1);
  layout->addWidget(reagents, 1, 2);
  setLayout(layout);
}
