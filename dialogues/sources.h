#ifndef SOURCESDIALOG_H
#define SOURCESDIALOG_H

#include <QDialog>
#include <QFileDialog>
#include <QMenu>
#include <QString>
#include <QTranslator>
#include <QTreeWidgetItem>
#include "dialogues/sources/create-source.h"
#include "widgets/a_button.h"
#include "widgets/a_containerslist.h"

/*!
 * Class: Sources.
 * Manages NLP module containers.
 */
class SourcesDialog : public QWidget {
 public:
  // Functions:
  explicit SourcesDialog(Basis *_basis, QWidget *parent = nullptr);
  void add();
  void remove();

 private:
  Q_DISABLE_COPY(SourcesDialog)

  // Objects:
  Basis *basis = nullptr;
  const QString objn = "containers";
  QMap<QTreeWidgetItem *, Source> csm;
  CreateSourceDialog *_cc = nullptr;
  AContainersList *cl = nullptr;
  QGridLayout *lt = nullptr;
  AButton *addBtn = nullptr;
  QAction *crtAct = nullptr;
  QAction *remAct = nullptr;
  AButton *snclBtn = nullptr;
  QAction *cancel = nullptr;
  bool edited = false;

  // Functions:
  void connector();
  void append(const QList<Source> &cProps);
  void appendSingle(const Source &cProp);
  void sncl();
  void openCC();
  void closeCC();
  void load();
};

#endif  // SOURCESDIALOG_H
