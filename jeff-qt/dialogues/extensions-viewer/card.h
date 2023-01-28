#ifndef EXTENSION_CARD_H
#define EXTENSION_CARD_H

#include "core-kit/extensions/extension.h"
#include "core-kit/extensions-manager.h"
#include "widgets/board.h"
#include "widgets/button.h"
#include "widgets/styling.h"
#include "widgets/layouts/linears.h"
#include <QLabel>
#include <QTimer>

/*! @class ExtensionsViewerCard
 *  @brief A card with a brief description of the extension.  */
class ExtensionsViewerCard : public Board {
  Q_OBJECT
  Q_DISABLE_COPY(ExtensionsViewerCard)
public:
  // Functions described in `card.cpp`:
  ExtensionsViewerCard(ExtensionMeta *_extension_meta, ExtensionsManager *_em, QWidget *parent = nullptr);
  void update();
  void update_status();

  /*! @brief Returns metadata about extension. */
  ExtensionMeta *meta() { return extension_meta; }

signals:
  /*! @brief Asks to open extension details. */
  ExtensionMeta *clicked_to_open(ExtensionMeta *extension_meta);

private:
  // Constants:
  const char *card_light_theme_style =
    "#board { border-width: 4px; border-style: solid; border-color: #f3f3f3;"
    "border-radius: 6px; background-color: #f3f3f3; } * { color: #16151d; }";
  const char *card_dark_theme_style =
    "#board { border-width: 4px; border-style: solid; border-color: #44495a;"
    "border-radius: 6px; background-color: #44495a; } * { color: #fcfcfc; }";
  
  // Objects:
  ExtensionsManager *em = nullptr;
  ExtensionMeta *extension_meta = nullptr;
  QLabel *extension_name = nullptr, *extension_description = nullptr, *extension_status = nullptr;
};

#endif
