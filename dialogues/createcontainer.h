#ifndef CREATECONTAINER_H
#define CREATECONTAINER_H

#include "dialogues/akiwake_dialog.h"
#include "widgets/akiwake_lineedit.h"
#include "widgets/akiwake_pushbutton.h"
#include <QGridLayout>
#include <QSpacerItem>
#include <QWidget>

class CreateContainer : public AkiwakeDialog {
public:
  explicit CreateContainer(QWidget *parent = nullptr);
private:
  Q_DISABLE_COPY(CreateContainer)
  void connector();
  AkiwakeLineEdit *containerName;
  AkiwakePushButton *save;
};

#endif // CREATECONTAINER_H
