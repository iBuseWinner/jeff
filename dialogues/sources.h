#ifndef SOURCESDIALOG_H
#define SOURCESDIALOG_H

#include "dialogues/modal-handler.h"
#include "dialogues/sources/create-source.h"
#include "widgets/button.h"
#include "widgets/menu.h"
#include "widgets/list.h"
#include <QApplication>
#include <QDialog>
#include <QFileDialog>
#include <QMap>
#include <QMenu>
#include <QString>
#include <QTranslator>
#include <QTreeWidgetItem>

/*!
 * @class SourcesDialog
 * @brief Manages NLP module's sources.
 */
class SourcesDialog : public QWidget {
  Q_OBJECT
  Q_DISABLE_COPY(SourcesDialog)
public:
  // Functions described in `sources.cpp`:
  SourcesDialog(Basis *_basis, QWidget *parent = nullptr, ModalHandler *m_handler = nullptr);
  void add();
  void remove();

private:
  // Objects:
  Basis *basis = nullptr;
  ModalHandler *_m_handler = nullptr;
  QMap<QTreeWidgetItem *, Source> source_widgets;
  CreateSourceDialog *create_source_dialog = nullptr;
  List *source_list = nullptr;
  QGridLayout *grid_layout = nullptr;
  Button *source_actions = nullptr;
  QAction *add_source = nullptr;
  QAction *create_source = nullptr;
  QAction *remove_source = nullptr;
  Button *save_and_close = nullptr;
  QAction *cancel = nullptr;
  bool edited = false;

  // Constants:
  inline static const QString object_name = "sources";

  // Functions described in `sources.cpp`:
  void connector();
  void append(Sources sources);
  void appendSingle(const Source &source);
  void sncl();
  void openCS();
  void closeCS();
  void load();
};

#endif
