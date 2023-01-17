#ifndef EXTENSION_BRIEF_H
#define EXTENSION_BRIEF_H

#include "core-kit/extensions/extension.h"
#include <QWidget>

/*! @class ExtensionsViewerBrief
 *  @brief TBD  */
class ExtensionsViewerBrief : public QWidget {
  Q_OBJECT
  Q_DISABLE_COPY(ExtensionsViewerBrief)
public:
  // Functions described in `brief.cpp`:
  ExtensionsViewerBrief(QWidget *parent = nullptr);
  void setup(ExtensionMeta *extension_meta);
  
signals:
  /*! @brief Asks to close brief and show overview again. */
  void close_brief();
};

#endif
