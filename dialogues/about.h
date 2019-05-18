#ifndef ABOUT_H
#define ABOUT_H

#include <QGridLayout>
#include <QLabel>
#include <QScrollArea>
#include <QSizePolicy>
#include <QTabWidget>
#include <QWidget>
#include "widgets/a_button.h"

/*!
 * Class: About
 * Contains information about the program and the authors.
 */
class About : public QWidget {
 public:
  // Functions:
  About(QWidget *parent = nullptr);

 private:
  Q_DISABLE_COPY(About)

  // Objects:
  const QString objn = "about";
  const int fw = 320;
  const int fh = 230;
};

#endif  // ABOUT_H
