#ifndef EXTENSIONS_VIEWER_H
#define EXTENSIONS_VIEWER_H

#include "core-kit/basis.hpp"
#include "core-kit/extensions-manager.hpp"
#include "dialogues/dialog.hpp"
#include "dialogues/modal-handler.hpp"
#include "dialogues/extensions-viewer/brief.hpp"
#include "dialogues/extensions-viewer/overview.hpp"
#include "widgets/button.hpp"
#include "widgets/scrollfreezerwidget.hpp"
#include "widgets/layouts/grid.hpp"

/*! @class ExtensionsViewer
 *  @brief Dialog to manage Jeff's extensions.  */
class ExtensionsViewer : public Dialog {
  Q_OBJECT
  Q_DISABLE_COPY(ExtensionsViewer)
public:
  // Functions described in `extensions-viewer.cpp`:
  ExtensionsViewer(ExtensionsManager *_em, QWidget *parent = nullptr, ModalHandler *mhandler = nullptr);
  
signals:
  /*! @brief TBD */
  QString show_info(QString text);
  
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
