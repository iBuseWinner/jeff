#ifndef EXTENSIONS_VIEWER_H
#define EXTENSIONS_VIEWER_H

#include "core-kit/basis.h"
#include "core-kit/extensions-manager.h"
#include "dialogues/dialog.h"
#include "dialogues/modal-handler.h"
#include "dialogues/extensions-viewer/brief.h"
#include "dialogues/extensions-viewer/overview.h"
#include "widgets/button.h"
#include "widgets/scrollfreezerwidget.h"
#include "widgets/layouts/grid.h"

/*! @class ExtensionsViewer
 *  @brief Dialog to manage Jeff's extensions.  */
class ExtensionsViewer : public Dialog {
  Q_OBJECT
  Q_DISABLE_COPY(ExtensionsViewer)
public:
  // Functions described in `extensions-viewer.cpp`:
  ExtensionsViewer(ExtensionsManager *_em, Basis *_basis,
                   QWidget *parent = nullptr, ModalHandler *mhandler = nullptr);
  
private:
  // Objects:
  ExtensionsViewerOverview *overview = nullptr;
  ExtensionsViewerBrief *brief = nullptr;
  GridLt *viewer_layout = nullptr;
  
  // Functions described in `extensions-viewer.cpp`:
  void open_brief_by_extension_meta(ExtensionMeta *extension_meta);
  void open_overview();
};

#endif 
