#ifndef CREATECONTAINER_H
#define CREATECONTAINER_H

#include <QFileDialog>
#include <QGridLayout>
#include <QRandomGenerator>
#include <QSpacerItem>
#include <QTime>
#include <QUuid>
#include <QWidget>
#include "core/containersstruct.h"
#include "widgets/a_lineedit.h"
#include "widgets/a_pushbutton.h"

class CreateContainer : public QWidget {
  Q_OBJECT
 public:
  CreateContainer(QWidget *p = nullptr);

 signals:
  void c();
  containerProperties cont(containerProperties _cProps);

 protected:
  void closeEvent(QCloseEvent *e) override;

 private:
  Q_DISABLE_COPY(CreateContainer)
  void connector();
  void selStart();
  void select();
  void save();
  QString m_dbpath = "";
  ALineEdit *t = nullptr;
  APushButton *sel = nullptr;
  APushButton *s = nullptr;
  APushButton *cc = nullptr;
};

#endif  // CREATECONTAINER_H
