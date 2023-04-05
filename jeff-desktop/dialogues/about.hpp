#ifndef ABOUT_H
#define ABOUT_H

#include "dialogues/modal-handler.hpp"
#include "widgets/button.hpp"
#include "widgets/scrollfreezerwidget.hpp"
#include "widgets/styling.hpp"
#include "widgets/layouts/grid.hpp"
#include "widgets/layouts/linears.hpp"
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
