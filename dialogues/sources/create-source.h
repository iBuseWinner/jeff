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

/*!
 * @class CreateSourceDialog
 * @brief Creates sources in databases.
 * @sa Basis, ALineEdit, AButton
 */
class CreateSourceDialog : public QWidget {
  Q_OBJECT
public:
  // Functions described in 'create-source.cpp':
  explicit CreateSourceDialog(Basis *_basis, QWidget *parent = nullptr);

signals:
  /*!
   * @brief Sends a signal that the user did not want to create a source.
   */
  void cancelled();

  /*!
   * @brief Sends source's data to create it.
   */
  const Source add(const Source &source);

protected:
  // Functions:
  /*! Cancels source's creation. */
  void closeEvent(QCloseEvent *event) override {
    emit cancelled();
    event->accept();
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

  // Functions described in 'create-source.cpp':
  void connector();
  void selStart();
  void select();
  void save();
};

#endif // CREATESOURCEDIALOG_H
