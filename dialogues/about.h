#ifndef ABOUT_H
#define ABOUT_H

#include "core/settingsstore.h"
#include "widgets/a_pushbutton.h"
#include <QGridLayout>
#include <QLabel>
#include <QScrollArea>
#include <QSizePolicy>
#include <QTabWidget>
#include <QWidget>

class About : public QWidget {
public:
  About(QWidget *parent = nullptr);
};

#endif // ABOUT_H
