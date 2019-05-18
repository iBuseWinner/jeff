#ifndef CREATECONTAINER_H
#define CREATECONTAINER_H

#include <QFileDialog>
#include <QGridLayout>
#include <QSpacerItem>
#include <QWidget>
#include "core/container.h"
#include "widgets/a_button.h"
#include "widgets/a_lineedit.h"

class CreateContainer : public QWidget {
  Q_OBJECT
 public:
  // Functions:
  CreateContainer(QWidget *parent = nullptr);

 signals:
  void cancelled();
  container completed(container _container);

 protected:
  // Functions:
  /*! Cancels container creation. */
  void closeEvent(QCloseEvent *e) override {
    emit cancelled();
    e->accept();
  }

 private:
  Q_DISABLE_COPY(CreateContainer)

  // Objects:
  QString m_dbpath = "";
  ALineEdit *titleInput = nullptr;
  AButton *selectFileBtn = nullptr;
  AButton *saveBtn = nullptr;
  AButton *cancelBtn = nullptr;

  // Functions:
  void connector();
  void selStart();
  void select();
  void save();
};

#endif  // CREATECONTAINER_H
