#ifndef CREATESOURCEDIALOG_H
#define CREATESOURCEDIALOG_H

#include "core-kit/basis.hpp"
#include "core-kit/model/source.hpp"
#include "widgets/button.hpp"
#include "widgets/lineedit.hpp"
#include <QFileDialog>
#include <QGridLayout>
#include <QSpacerItem>
#include <QWidget>

/*!
 * @class CreateSourceDialog
 * @brief Creates sources in databases.
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
  Source add(Source source);

protected:
  // Functions:
  /*! @brief Cancels source's creation. */
  void closeEvent(QCloseEvent *event) override { emit cancelled(); event->accept(); }

private:
  // Objects:
  QString path = "";
  Basis *basis = nullptr;
  QGridLayout layout;
  LineEdit title_edit;
  Button select, save, cancel;

  // Functions described in `create-source.cpp`:
  void fill_select_file_btn();
  void select_file();
  void create_source();
};

#endif
