#ifndef SOURCESDIALOG_H
#define SOURCESDIALOG_H

#include "dialogues/dialog.hpp"
#include "dialogues/modal-handler.hpp"
#include "dialogues/sources-editor/create-source.hpp"
#include "widgets/button.hpp"
#include "widgets/list.hpp"
#include "widgets/layouts/grid.hpp"
#include <QApplication>
#include <QDialog>
#include <QFileDialog>
#include <QMap>
#include <QMenu>
#include <QString>
#include <QTranslator>
#include <QTreeWidgetItem>

/*! @class SourcesEditor
 *  @brief Manages NLP module's sources.  */
class SourcesEditor : public Dialog {
  Q_OBJECT
  Q_DISABLE_COPY(SourcesEditor)
public:
  // Functions described in `sources-editor.cpp`:
  SourcesEditor(Basis *_basis, QWidget *parent = nullptr, ModalHandler *mhandler = nullptr);
  void add();
  void remove();

private:
  // Objects:
  Basis *basis = nullptr;
  QMap<QTreeWidgetItem *, Source> source_widgets;
  CreateSourceDialog *create_source_dialog = nullptr;
  List *source_list = nullptr;
  GridLt *layout = nullptr;
  Button *source_actions_btn = nullptr;
  QAction *add_source = nullptr;
  QAction *create_source = nullptr;
  QAction *remove_source = nullptr;
  Button *save_and_close_btn = nullptr;
  QAction *cancel = nullptr;
  bool edited = false;

  // Constants:
  const char *object_name = "sources_editor";

  // Functions described in `sources-editor.cpp`:
  void connector();
  void append(Sources sources);
  void appendSingle(Source source);
  void save_and_close();
  void open_create_source();
  void close_create_source();
  void load();
};

#endif
