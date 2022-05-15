#ifndef PHRASE_EDITOR_EXEC_H
#define PHRASE_EDITOR_EXEC_H

#include "core/basis.h"
#include "core/model/python/script.h"
#include "widgets/button.h"
#include "widgets/lineedit.h"
#include "widgets/list.h"
#include "widgets/spoiler.h"
#include <QFileDialog>
#include <QGridLayout>
#include <QJsonObject>
#include <QLabel>
#include <QList>
#include <QSpinBox>
#include <QTreeWidgetItem>
#include <QWidget>

/*!
 * @class PhraseEditorEditAsReactScript
 * @brief TBD
 */
class PhraseEditorEditAsReactScript : public QWidget {
  Q_OBJECT
  Q_DISABLE_COPY(PhraseEditorEditAsReactScript)
public:  
  // Functions described in `edit-phrase-as-reactscript.cpp`:
  PhraseEditorEditAsReactScript(Basis *_basis, QWidget *parent = nullptr);
  ~PhraseEditorEditAsReactScript();
  void load_from_text(QString expression);
  
signals:
  /*! @brief TBD */
  void save(QString expression);
  
private:
  // Objects:
  Basis *basis = nullptr;
  QLabel choose_path, specify_func_name, specify_amount_of_history, specify_memory_cells;
  Button path, add_memory_cell_btn, remove_memory_cell_btn, save_script_btn;
  LineEdit func_name;
  QSpinBox history_amount;
  Spoiler other_props_widget;
  QGridLayout memory_cells_manager_layout, other_props_widget_layout, editor_layout;
  QWidget memory_cells_manager_widget;
  List memory_cells;
  QList<QTreeWidgetItem *> memory_cells_items;
  ReactScript *script = nullptr;
  
  // Functions described in `edit-phrase-as-reactscript.cpp`:
  void select_file();
  void clear();
  void save_script();
};

#endif
