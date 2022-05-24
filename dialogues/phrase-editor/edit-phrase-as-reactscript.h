#ifndef PHRASE_EDITOR_EXEC_H
#define PHRASE_EDITOR_EXEC_H

#include "core/basis.h"
#include "core/model/python/script.h"
#include "widgets/button.h"
#include "widgets/lineedit.h"
#include "widgets/list.h"
#include "widgets/spoiler.h"
#include <QCheckBox>
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
 * @brief Edits a phrase like a script.
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
  /*! @brief Notifies that editing is complete and returns the text to be saved to the database. */
  void save(QString expression);
  
private:
  // Objects:
  Basis *basis = nullptr;
  QLabel choose_path, specify_func_name, specify_amount_of_history, specify_memory_cells;
  Button path, save_script_btn;
  LineEdit func_name;
  QSpinBox history_amount;
  QCheckBox needs_user_input;
  Spoiler other_props_widget;
  QGridLayout other_props_widget_layout, editor_layout;
  EditList *memory_cells = nullptr;
  ReactScript *script = nullptr;
  
  // Functions described in `edit-phrase-as-reactscript.cpp`:
  void clear();
  void save_script();
};

#endif
