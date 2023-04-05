#ifndef EXTENSION_BRIEF_H
#define EXTENSION_BRIEF_H

#include "core-kit/extensions-manager.hpp"
#include "core-kit/extensions/extension.hpp"
#include "widgets/button.hpp"
#include "widgets/menu.hpp"
#include "widgets/layouts/linears.hpp"
#include <QAction>
#include <QClipboard>
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
  /*! @brief Causes the display of detailed information about the extension. */
  QString show_info(QString text);

private:
  // Objects:
  ExtensionsManager *em = nullptr;
  ExtensionMeta *extension_meta = nullptr;
  QLabel name_lbl, appeal_lbl, desc_lbl, authors_lbl, license_lbl, links_lbl, status_lbl;
  Button back_btn, on_off_btn, remove_btn, print_stdout_btn, print_stderr_btn;
  
  // Functions described in `brief.cpp`:
  void update_status();
  void remove_extension();
  void get_ext_stdout();
  void get_ext_stderr();
};

#endif
