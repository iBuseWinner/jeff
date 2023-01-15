#include "extensions-viewer.h"

/*! @brief The constructor. */
ExtensionsViewer::ExtensionsViewer(
  ExtensionsManager *_em, Basis *_basis, QWidget *parent, ModalHandler *m_handler
) : ScrollFreezerWidget(parent) {
  m_handler->setPrisoner(this);
  // Sets up overview widget.
  overview = new ExtensionsViewerOverview(_em, _basis, this);
  // Shows overview.
  viewer_layout = GridLt::another()->spacing()->addw(overview);
  setLayout(viewer_layout);
  setFixedWidth(480);
}
