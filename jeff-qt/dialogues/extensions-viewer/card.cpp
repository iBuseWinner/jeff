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
) : QWidget(parent), em(_em), extension_meta(_extension_meta) {
  auto *open_btn = new Button(tr("See details"), this);
  auto *extension_name = new QLabel(extension_meta->name, this);
  auto *extension_description = new QLabel(extension_meta->desc, this);
  auto *extension_status = new QLabel(this);
  extension_status->setTextFormat(Qt::RichText);
  if (em->is_running(extension_meta))
    extension_status->setText("<p style=\"color:#3b961a\">" + tr("Running") + "</p>");
  else
    extension_status->setText("<p style=\"color:#d22a2a\">" + tr("Stopped") + "</p>");
  setLayout(VLineLt::another()
    ->addw(extension_name)->addw(extension_description)
    ->addlt(
      HLineLt::another()->addw(extension_status)->addw(open_btn)
    )
  );
  connect(open_btn, &Button::clicked, this, [this] { emit clicked_to_open(extension_meta); });
}
