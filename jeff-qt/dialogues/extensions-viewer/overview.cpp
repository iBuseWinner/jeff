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
  connect(add_extension_btn, &Button::clicked, this, &ExtensionsViewerOverview::read_from_file);
  connect(close_btn, &Button::clicked, this, [this] { emit close_viewer(); });
  viewer_list_lt = VLineLt::another();
  auto *scroll_area = new ScrollArea(this);
  auto *area_widget = new QWidget(this);
  area_widget->setContentsMargins(0, 0, 5, 0);
  area_widget->setLayout(viewer_list_lt);
  scroll_area->setWidget(area_widget);
  setLayout(
    VLineLt::another()->spacing(3)->addw(extensions_header)
      ->addw(scroll_area)
      ->addlt(HLineLt::another()->addw(add_extension_btn)->addw(close_btn))
  );
  fill_extensions_cards();
}

/*! @brief Shows all extensions used by Jeff. */
void ExtensionsViewerOverview::fill_extensions_cards() {
  auto extensions_meta = em->get_extensions_meta();
  if (no_extensions_yet) {
    viewer_list_lt->rem(no_extensions_yet);
    delete no_extensions_yet;
  } else for (auto *existing_card : cards) {
    disconnect(existing_card, &ExtensionsViewerCard::clicked_to_open, nullptr, nullptr);
    viewer_list_lt->removeWidget(existing_card);
    existing_card->close();
    delete existing_card;
  }
  cards.clear();
  if (extensions_meta.isEmpty()) {
    no_extensions_yet = new QLabel("<i>" + tr("Jeff doesn't have extensions yet. You can add them yourself by downloading from the Internet and clicking on the \"Add from file\" button.") + "</i>", this);
    no_extensions_yet->setWordWrap(true);
    no_extensions_yet->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    viewer_list_lt->addWidget(no_extensions_yet);
    return;
  } else for (auto *extension_meta : extensions_meta) {
    auto *card = new ExtensionsViewerCard(extension_meta, em, this);
    connect(card, &ExtensionsViewerCard::clicked_to_open, this, [this, extension_meta] {
      emit open_brief_by_extension_meta(extension_meta);
    });
    viewer_list_lt->addWidget(card);
    cards.append(card);
  }
}

/*! @brief TBD */
void ExtensionsViewerOverview::read_from_file() {
  QString filename = QFileDialog::getOpenFileName(
    nullptr, tr("Select extension's configuration"), nullptr, tr("Jeff's extension") + "(extension.json)"
  );
  if (filename.isEmpty()) return;
}
