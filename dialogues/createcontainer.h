#ifndef CREATECONTAINER_H
#define CREATECONTAINER_H

#include "widgets/a_lineedit.h"
#include "widgets/a_pushbutton.h"
#include <QGridLayout>
#include <QSpacerItem>
#include <QWidget>

class CreateContainer : public QWidget {
  Q_OBJECT
public:
  CreateContainer(QWidget *parent = nullptr);

signals:
  void closingSignal();

protected:
  void closeEvent(QCloseEvent *event) override;

private:
  Q_DISABLE_COPY(CreateContainer)
  void connector();
  ALineEdit *containerName = nullptr;
  APushButton *databasePath = nullptr;
  APushButton *save = nullptr;
  APushButton *cancel = nullptr;
};

#endif // CREATECONTAINER_H
