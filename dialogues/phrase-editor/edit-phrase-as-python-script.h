#ifndef PHRASE_EDITOR_EXEC_H
#define PHRASE_EDITOR_EXEC_H

#include "core/basis.h"
#include "widgets/button.h"
#include "widgets/lineedit.h"
#include "widgets/list.h"
#include "widgets/spoiler.h"
#include <QFileDialog>
#include <QGridLayout>
#include <QJsonObject>
#include <QLabel>
#include <QSpinBox>
#include <QWidget>

/*!
 * @class PhraseEditorEditAsPython
 * @brief TBD
 */
class PhraseEditorEditAsPython : public QWidget {
  Q_OBJECT
  Q_DISABLE_COPY(PhraseEditorEditAsPython)
public:
  // Objects:
  QString address;
  
  // Functions described in `edit-phrase-as-python-script.cpp`:
  PhraseEditorEditAsPython(Basis *_basis, QWidget *parent = nullptr);
  void clear();
  
signals:
  /*! @brief TBD */
  void save();
  
private:
  // Objects:
  Basis *basis = nullptr;
  QLabel choose_path, specify_func_name, specify_amount_of_history, specify_memory_cells;
  Button path, add_memory_cell, remove_memory_cell, save_script;
  LineEdit func_name;
  Spoiler other_props;
  QSpinBox history_amount;
  List memory_cells;
};

#endif
