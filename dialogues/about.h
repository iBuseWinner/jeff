#ifndef ABOUT_H
#define ABOUT_H

#include "dialogues/modal-handler.h"
#include "widgets/button.h"
#include "widgets/styling.h"
#include <QApplication>
#include <QCoreApplication>
#include <QGridLayout>
#include <QLabel>
#include <QPalette>
#include <QPixmap>
#include <QScrollArea>
#include <QSizePolicy>
#include <QTabWidget>
#include <QTranslator>
#include <QWidget>

/*!
 * @class About
 * @brief Contains information about the program and the authors.
 * @sa ModalHandler
 */
class About : public QWidget {
  Q_DISABLE_COPY(About)
public:
  // Functions described in `about.cpp`:
  About(QWidget *parent = nullptr, ModalHandler *m_handler = nullptr);

private:
  // Objects:
  ModalHandler *_m_handler = nullptr;

  // Constants:
  inline static const QString object_name = "about";
  static const int fixed_width = 320;
  static const int fixed_height = 230;
};

#endif
