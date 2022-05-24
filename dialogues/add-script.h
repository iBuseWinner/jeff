#ifndef SCRIPT_DIALOG_H
#define SCRIPT_DIALOG_H

#include "core/basis.h"
#include "core/python-module.h"
#include "dialogues/modal-handler.h"
#include "widgets/button.h"
#include "widgets/combobox.h"
#include "widgets/editlist.h"
#include "widgets/lineedit.h"
#include "widgets/list.h"
#include "widgets/scrollfreezerwidget.h"
#include <QCheckBox>
#include <QFileDialog>
#include <QGridLayout>
#include <QLabel>
#include <QLayoutItem>
#include <QSpinBox>

/*! @class AddScriptDialog
 *  @brief Adds information about scripts to Jeff.  */
class AddScriptDialog : public ScrollFreezerWidget {
  Q_OBJECT
  Q_DISABLE_COPY(AddScriptDialog)
public:  
  // Functions described in `add-script.cpp`:
  AddScriptDialog(QWidget *parent = nullptr, Basis *_basis = nullptr, 
                  PythonModule *_pm = nullptr, ModalHandler *m_handler = nullptr);
  ~AddScriptDialog();
  bool load_from_text(QString json_text);
  bool load_from_script(ScriptMetadata *script);
  
signals:
  /*! @brief Notifies the parent that editing is complete (you can close the widget) 
   *  and sends it information about the script.  */
  QString saved(QString json_script);
  void closed();
  ScriptMetadata *load(ScriptMetadata *script);
  
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
  
  // Functions described in `add-script.cpp`:
  void change_stype();
  void change_stype(int _stype);
  void set_stype(int _stype);
};

#endif
