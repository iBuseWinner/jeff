#ifndef CREATESOURCEDIALOG_H
#define CREATESOURCEDIALOG_H

#include "core/basis.h"
#include "core/source.h"
#include "widgets/a_button.h"
#include "widgets/a_lineedit.h"
#include <QFileDialog>
#include <QGridLayout>
#include <QSpacerItem>
#include <QWidget>

class CreateSourceDialog : public QWidget {
  Q_OBJECT
public:
  // Functions:
  explicit CreateSourceDialog(Basis *_basis, QWidget *parent = nullptr);

signals:
  void cancelled();
  const Source add(const Source &cProp);

protected:
  // Functions:
  /*! Cancels container creation. */
  void closeEvent(QCloseEvent *e) override {
    emit cancelled();
    e->accept();
  }

private:
  Q_DISABLE_COPY(CreateSourceDialog)

  // Objects:
  QString m_dbpath = "";
  Basis *basis = nullptr;
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

#endif // CREATESOURCEDIALOG_H
