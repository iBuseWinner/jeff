#ifndef EXTENSIONS_OVERVIEW_H
#define EXTENSIONS_OVERVIEW_H

#include "core-kit/basis.h"
#include "core-kit/extensions-manager.h"
#include "core-kit/extensions/extension.h"
#include "dialogues/extensions-viewer/card.h"
#include "widgets/layouts/linears.h"
#include <QList>
#include <QWidget>

/*! @class ExtensionsViewerOverview
 *  @brief List of extension cards with the ability to add new ones.  */
class ExtensionsViewerOverview : public QWidget {
  Q_OBJECT
  Q_DISABLE_COPY(ExtensionsViewerOverview)
public:
  // Functions described in `overview.cpp`:
  ExtensionsViewerOverview(ExtensionsManager *_em, Basis *_basis, QWidget *parent = nullptr);
  void fill_extensions_cards();

signals:
  /*! @brief Asks to open extension details. */
  ExtensionMeta *open_brief_by_extension_meta(ExtensionMeta *extension_meta);
  
private:
  // Objects:
  ExtensionsManager *em = nullptr;
  Basis *basis = nullptr;
  VLineLt *viewer_list_lt = nullptr;
  QList<ExtensionsViewerCard *> cards;
};

#endif
