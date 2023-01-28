#include "extensions-viewer.h"

/*! @brief The constructor. */
ExtensionsViewer::ExtensionsViewer(
  ExtensionsManager *_em, QWidget *parent, ModalHandler *mhandler
) : Dialog(mhandler, parent) {
  // Sets up overview widget.
  overview = new ExtensionsViewerOverview(_em, this);
  overview->setFixedWidth(480);
  overview->setFixedHeight(320);
  connect(overview, &ExtensionsViewerOverview::open_brief_by_extension_meta,
          this, &ExtensionsViewer::open_brief_by_extension_meta);
  connect(overview, &ExtensionsViewerOverview::close_viewer, this, &Dialog::close);
  // Sets up brief widget.
  brief = new ExtensionsViewerBrief(_em, this);
  brief->setFixedWidth(480);
  connect(brief, &ExtensionsViewerBrief::close_brief, this, &ExtensionsViewer::open_overview);
  brief->hide();
  // Shows overview.
  viewer_layout = GridLt::another()->spacing()->addw(overview);
  setLayout(viewer_layout);
}

/*! @brief Shows the extension's details. */
void ExtensionsViewer::open_brief_by_extension_meta(ExtensionMeta *extension_meta) {
  overview->hide();
  viewer_layout->replaceWidget(overview, brief);
  brief->setup(extension_meta);
  dynamic_cast<QWidget *>(parent())->setFixedWidth(500);
  brief->show();
}

/*! @brief Shows the overview. */
void ExtensionsViewer::open_overview() {
  brief->hide();
  viewer_layout->replaceWidget(brief, overview);
  overview->fill_extensions_cards();
  overview->show();
}
