#ifndef ABOUT_H
#define ABOUT_H

#include <QGridLayout>
#include <QLabel>
#include <QScrollArea>
#include <QSizePolicy>
#include <QTabWidget>
#include <QWidget>
#include "widgets/a_button.h"

class About : public QWidget {
 public:
  About(QWidget *p = nullptr);

 private:
  Q_DISABLE_COPY(About)
  const int fw = 320;
  const int fh = 230;
};

#endif  // ABOUT_H
