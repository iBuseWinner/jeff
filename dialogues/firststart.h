#ifndef FIRSTSTART_H
#define FIRSTSTART_H

#include <QGridLayout>
#include <QLabel>
#include <QSpacerItem>
#include <QTranslator>
#include <QWidget>
#include "widgets/a_button.h"

/*!
 * Class: FirstStart
 * Shows a quick start guide.
 */
class FirstStart : public QWidget {
 public:
  // Functions:
  FirstStart(QWidget *parent = nullptr);

 private:
  Q_DISABLE_COPY(FirstStart)

  // Objects:
  const QString objn = "firststart";
};

#endif  // FIRSTSTART_H
