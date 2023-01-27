#ifndef EXTENSION_BRIEF_H
#define EXTENSION_BRIEF_H

#include "core-kit/extensions-manager.h"
#include "core-kit/extensions/extension.h"
#include "widgets/button.h"
#include "widgets/layouts/linears.h"
#include <QLabel>
#include <QSpacerItem>
#include <QTimer>
#include <QWidget>

/*! @class ExtensionsViewerBrief
 *  @brief An extended Jeff extension' card with some options.  */
class ExtensionsViewerBrief : public QWidget {
  Q_OBJECT
  Q_DISABLE_COPY(ExtensionsViewerBrief)
public:
  // Functions described in `brief.cpp`:
  ExtensionsViewerBrief(ExtensionsManager *_em, QWidget *parent = nullptr);
  void setup(ExtensionMeta *_extension_meta);

signals:
  /*! @brief Asks to close brief and show overview again. */
  void close_brief();

private:
  // Objects:
  ExtensionsManager *em = nullptr;
  ExtensionMeta *extension_meta = nullptr;
  QLabel name_lbl, desc_lbl, authors_lbl, license_lbl, links_lbl, status_lbl;
  Button back_btn, remove_btn, on_off_btn;
  
  // Functions described in `brief.cpp`:
  void update_status();
  void remove_extension();
};

#endif
