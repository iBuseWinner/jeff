#ifndef CREATECONTAINER_H
#define CREATECONTAINER_H

#include "widgets/a_lineedit.h"
#include "widgets/a_pushbutton.h"
#include <QGridLayout>
#include <QSpacerItem>
#include <QWidget>

class CreateContainer : public QWidget {
public:
  CreateContainer(QWidget *parent = nullptr);
private:
  Q_DISABLE_COPY(CreateContainer)
  // void connector();
  ALineEdit *containerName = nullptr;
  APushButton *save = nullptr;
};

#endif // CREATECONTAINER_H
