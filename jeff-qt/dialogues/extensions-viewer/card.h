#ifndef EXTENSION_CARD_H
#define EXTENSION_CARD_H

#include "core-kit/extensions/extension.h"
#include "core-kit/extensions-manager.h"
#include "widgets/button.h"
#include "widgets/layouts/linears.h"
#include <QLabel>
#include <QWidget>

/*! @class ExtensionsViewerCard
 *  @brief A card with a brief description of the extension.  */
class ExtensionsViewerCard : public QWidget {
  Q_OBJECT
  Q_DISABLE_COPY(ExtensionsViewerCard)
public:
  // Functions described in `card.cpp`:
  ExtensionsViewerCard(ExtensionMeta *_extension_meta, ExtensionsManager *_em, QWidget *parent = nullptr);

signals:
  /*! @brief Asks to open extension details. */
  ExtensionMeta *clicked_to_open(ExtensionMeta *extension_meta);

private:
  // Objects:
  ExtensionsManager *em = nullptr;
  ExtensionMeta *extension_meta = nullptr;
};

#endif
