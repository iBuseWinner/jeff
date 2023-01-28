#include "card.h"

/*! @brief The constructor.
 *  @details Widget's layout:
 *  <---------------------->
 *  [      Ext's name      ]
 *  [   Some description   ]
 *  [Status]{<> See details}
 *  <---------------------->  */
ExtensionsViewerCard::ExtensionsViewerCard(
  ExtensionMeta *_extension_meta, ExtensionsManager *_em, QWidget *parent
) : Board(parent), em(_em), extension_meta(_extension_meta) {
  setStyleSheet(styling.light_theme ? card_light_theme_style : card_dark_theme_style);
  auto *open_btn = new Button(tr("See details"), this);
  extension_name = new QLabel("<b>" + extension_meta->name + "</b>", this);
  extension_name->setTextFormat(Qt::RichText);
  extension_description = new QLabel(extension_meta->desc, this);
  extension_description->setWordWrap(true);
  extension_status = new QLabel(this);
  extension_status->setTextFormat(Qt::RichText);
  extension_status->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  update_status();
  setLayout(VLineLt::another()
    ->addw(extension_name)->addw(extension_description)
    ->addlt(
      HLineLt::another()->addw(extension_status)->addw(open_btn)
    )
  );
  connect(open_btn, &Button::clicked, this, [this] { emit clicked_to_open(extension_meta); });
}

/*! @brief Updates the card with updated metadata if needed. */
void ExtensionsViewerCard::update() {
  extension_name->setText(extension_meta->name);
  extension_description->setText(extension_meta->desc);
  update_status();
}

/*! @brief Updates running status. */
void ExtensionsViewerCard::update_status() {
  if (em->is_running(extension_meta))
    extension_status->setText("<p style=\"color:#3b961a\">" + tr("Running") + "</p>");
  else
    extension_status->setText("<p style=\"color:#d22a2a\">" + tr("Stopped") + "</p>");
  QTimer::singleShot(1000, this, &ExtensionsViewerCard::update_status);
}
