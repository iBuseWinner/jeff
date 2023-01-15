#include "overview.h"

/*! @brief The constructor. 
 *  @details Widget's layout:
 *  <------------------------>
 *  [         Header         ]
 *  <      List of cards     >
 *  { Add from file }{ Close }
 *  <------------------------>  */
ExtensionsViewerOverview::ExtensionsViewerOverview(ExtensionsManager *_em, Basis *_basis, QWidget *parent)
    : QWidget(parent), em(_em), basis(_basis) {
  auto *extensions_header = new QLabel("<h1><b>" + tr("Extensions") + "</b></h1>", this);
  auto *add_extension_btn = new Button(tr("Add from file"), this);
  auto *close_btn = new Button(tr("Close"), this);
  add_extension_btn->setIcon(
    QIcon::fromTheme("list-add", QIcon(":/arts/icons/16/list-add.svg")));
  close_btn->setIcon(
    QIcon::fromTheme("collapse-all", QIcon(":/arts/icons/16/collapse-all.svg")));
  setLayout(
    VLineLt::another()->margin(6)->addw(extensions_header)
      ->addlt(VLineLt::another())
      ->addlt(HLineLt::another()->addw(add_extension_btn)->addw(close_btn))
  );
  fill_extensions_cards();
}

/*! @brief Shows all extensions used by Jeff. */
void ExtensionsViewerOverview::fill_extensions_cards() {
  auto extensions_meta = em->get_extensions_meta();
  for (auto *existing_card : cards) {
    disconnect(existing_card, &ExtensionsViewerCard::clicked_to_open, nullptr, nullptr);
    viewer_list_lt->removeWidget(existing_card);
    existing_card->close();
    delete existing_card;
  }
  cards.clear();
  for (auto *extension_meta : extensions_meta) {
    auto *card = new ExtensionsViewerCard(extension_meta, em, this);
    connect(card, &ExtensionsViewerCard::clicked_to_open, this, [this, extension_meta] {
      emit open_brief_by_extension_meta(extension_meta);
    });
    viewer_list_lt->addWidget(card);
    cards.append(card);
  }
}
