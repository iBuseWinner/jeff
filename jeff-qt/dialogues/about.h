#ifndef ABOUT_H
#define ABOUT_H

#include "dialogues/modal-handler.h"
#include "widgets/button.h"
#include "widgets/scrollfreezerwidget.h"
#include "widgets/styling.h"
#include "widgets/layouts/grid.h"
#include "widgets/layouts/linears.h"
#include <QApplication>
#include <QCoreApplication>
#include <QLabel>
#include <QPalette>
#include <QPixmap>
#include <QScrollArea>
#include <QSizePolicy>
#include <QTabWidget>
#include <QTranslator>
#include <QWidget>

/*! @class About
 *  @brief Contains information about the program and the authors.  */
class About : public ScrollFreezerWidget {
  Q_OBJECT
  Q_DISABLE_COPY(About)
public:
  // Functions described in `about.cpp`:
  About(QWidget *parent = nullptr, ModalHandler *mhandler = nullptr);

private:
  // Constants:
  static const int fixed_width = 320;
};

#endif
