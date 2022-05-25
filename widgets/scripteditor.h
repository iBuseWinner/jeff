#ifndef SCRIPT_EDITOR_H
#define SCRIPT_EDITOR_H

#include "core/basis.h"
#include "core/python-module.h"
#include "dialogues/modal-handler.h"
#include "widgets/button.h"
#include "widgets/combobox.h"
#include "widgets/editlist.h"
#include "widgets/lineedit.h"
#include "widgets/list.h"
#include <QCheckBox>
#include <QFileDialog>
#include <QGridLayout>
#include <QLabel>
#include <QLayoutItem>
#include <QSpinBox>

/*! @class ScriptEditor
 *  @brief Adds information about scripts to Jeff.  */
class ScriptEditor : public QWidget {
  Q_OBJECT
  Q_DISABLE_COPY(ScriptEditor)
public:  
  // Functions described in `scripteditor.cpp`:
  ScriptEditor(QWidget *parent = nullptr, Basis *_basis = nullptr, 
               PythonModule *_pm = nullptr, ModalHandler *m_handler = nullptr);
  ~ScriptEditor();
  bool load_from_text(QString json_text);
  bool load_from_script(ScriptMetadata *script);
  
signals:
  /*! @brief Notifies the parent that editing is complete (you can close the widget) 
   *  and sends it information about the script.  */
  QString saved(QString json_script);
  void closed();
  
private:
  // Objects:
  int stype = 0;
  Basis *basis = nullptr;
  PythonModule *pm = nullptr;
  ModalHandler *_m_handler = nullptr;
  Button *path_input = nullptr, *save_btn = nullptr;
  QLabel *stype_info = nullptr;
  ComboBox *stype_input = nullptr;
  QGridLayout *dynamic_properties_layout = nullptr;
  /*! @brief Shared objects that used for loading states from JSON. */
  LineEdit *fn_name_input = nullptr;
  EditList *memory_cells_list = nullptr;
  QCheckBox *needs_ue_input = nullptr;
  QSpinBox *hist_amount_input = nullptr;
  
  // Functions described in `scripteditor.cpp`:
  void change_stype();
  void change_stype(int _stype);
  void set_stype(int _stype);
};

#endif
