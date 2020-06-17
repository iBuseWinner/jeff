#ifndef ABOUT_H
#define ABOUT_H

#include "widgets/a_button.h"
#include <QCoreApplication>
#include <QGridLayout>
#include <QLabel>
#include <QScrollArea>
#include <QSizePolicy>
#include <QTabWidget>
#include <QTranslator>
#include <QWidget>

/*!
 * Class: About.
 * Contains information about the program and the authors.
 */
class About : public QWidget {
public:
  // Functions:
  explicit About(QWidget *parent = nullptr);

private:
  Q_DISABLE_COPY(About)

  // Objects:
  inline static const QString objn = "about";
  static const int fixed_width = 320;
  static const int fixed_height = 230;
};

#endif // ABOUT_H
