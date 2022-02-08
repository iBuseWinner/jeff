#ifndef CREATESOURCEDIALOG_H
#define CREATESOURCEDIALOG_H

#include "core/basis.h"
#include "core/model/source.h"
#include "widgets/button.h"
#include "widgets/lineedit.h"
#include <QFileDialog>
#include <QGridLayout>
#include <QSpacerItem>
#include <QWidget>

/*!
 * @class CreateSourceDialog
 * @brief Creates sources in databases.
 * @sa Basis, LineEdit, Button
 */
class CreateSourceDialog : public QWidget {
  Q_OBJECT
  Q_DISABLE_COPY(CreateSourceDialog)
public:
  // Functions described in `create-source.cpp`:
  CreateSourceDialog(Basis *_basis, QWidget *parent = nullptr);

signals:
  /*! @brief Sends a signal that the user did not want to create a source. */
  void cancelled();
  /*! @brief Sends source's data to create it. */
  const Source add(const Source &source);

protected:
  // Functions:
  /*! @brief Cancels source's creation. */
  void closeEvent(QCloseEvent *event) override { emit cancelled(); event->accept(); }

private:
  // Objects:
  QString m_dbpath = "";
  Basis *basis = nullptr;
  LineEdit *titleInput = nullptr;
  Button *selectFileBtn = nullptr;
  Button *saveBtn = nullptr;
  Button *cancelBtn = nullptr;

  // Functions described in `create-source.cpp`:
  void connector();
  void selStart();
  void select();
  void save();
};

#endif
