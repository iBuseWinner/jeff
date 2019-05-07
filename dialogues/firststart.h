#ifndef FIRSTSTART_H
#define FIRSTSTART_H

#include <QGridLayout>
#include <QLabel>
#include <QSpacerItem>
#include <QWidget>
#include "widgets/a_button.h"

class FirstStart : public QWidget {
 public:
  FirstStart(QWidget *p = nullptr);

 private:
  Q_DISABLE_COPY(FirstStart)
};

#endif  // FIRSTSTART_H
