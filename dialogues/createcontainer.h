#ifndef CREATECONTAINER_H
#define CREATECONTAINER_H

#include <QFileDialog>
#include <QGridLayout>
#include <QRandomGenerator>
#include <QSpacerItem>
#include <QTime>
#include <QUuid>
#include <QWidget>
#include "core/container.h"
#include "widgets/a_lineedit.h"
#include "widgets/a_button.h"

class CreateContainer : public QWidget {
  Q_OBJECT
 public:
  CreateContainer(QWidget *p = nullptr);

 signals:
  void c();
  container cont(container _cProps);

 protected:
  void closeEvent(QCloseEvent *e) override;

 private:
  Q_DISABLE_COPY(CreateContainer)
  QString m_dbpath = "";
  ALineEdit *t = nullptr;
  AButton *sel = nullptr;
  AButton *s = nullptr;
  AButton *cc = nullptr;
  void connector();
  void selStart();
  void select();
  void save();
};

#endif  // CREATECONTAINER_H
