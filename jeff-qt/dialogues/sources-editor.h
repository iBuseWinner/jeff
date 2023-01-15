#ifndef SOURCESDIALOG_H
#define SOURCESDIALOG_H

#include "dialogues/modal-handler.h"
#include "dialogues/sources-editor/create-source.h"
#include "widgets/button.h"
#include "widgets/menu.h"
#include "widgets/list.h"
#include "widgets/scrollfreezerwidget.h"
#include "widgets/layouts/grid.h"
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
class SourcesEditor : public ScrollFreezerWidget {
  Q_OBJECT
  Q_DISABLE_COPY(SourcesEditor)
public:
  // Functions described in `sources-editor.cpp`:
  SourcesEditor(Basis *_basis, QWidget *parent = nullptr, ModalHandler *m_handler = nullptr);
  void add();
  void remove();

private:
  // Objects:
  Basis *basis = nullptr;
  ModalHandler *_m_handler = nullptr;
  QMap<QTreeWidgetItem *, Source> source_widgets;
  CreateSourceDialog *create_source_dialog = nullptr;
  List *source_list = nullptr;
  GridLt *layout = nullptr;
  Button *source_actions = nullptr;
  QAction *add_source = nullptr;
  QAction *create_source = nullptr;
  QAction *remove_source = nullptr;
  Button *save_and_close = nullptr;
  QAction *cancel = nullptr;
  bool edited = false;

  // Constants:
  const char *object_name = "sources_editor";

  // Functions described in `sources-editor.cpp`:
  void connector();
  void append(Sources sources);
  void appendSingle(Source source);
  void sncl();
  void openCS();
  void closeCS();
  void load();
};

#endif
