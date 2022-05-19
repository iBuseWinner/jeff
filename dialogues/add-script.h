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
#include <QCheckBox>
#include <QGridLayout>
#include <QLabel>
#include <QWidget>

/*! @class AddScriptDialog
 *  @brief TBD  */
class AddScriptDialog : public QWidget {
  Q_OBJECT
  Q_DISABLE_COPY(AddScriptDialog)
public:
  // Objects:
  ComboBox stype_input;
  
  // Functions described in `add-script.cpp`:
  AddScriptDialog(QWidget *parent, Basis *_basis, PythonModule *_pm, ModalHandler *m_handler);
  void change_stype();
  
signals:
  /*! @brief TBD */
  QString saved(QString json_script);
  
private:
  // Objects:
  Basis *basis = nullptr;
  PythonModule *pm = nullptr;
  ModalHandler *_m_handler = nullptr;
  QLabel path_info, stype_info;
  Button path_input;
  QLabel fn_name_info, send_adprops_info, server_addr_info, server_port_info, memory_cells_info;
  QCheckBox send_adprops_input;
  LineEdit fn_name_input, server_addr_input, server_port_input;
  EditListWidget edit_list;
  Button cancel_btn, save_btn;
  QWidget dynamic_properties_widget, buttons_widget;
  QGridLayout main_layout, dynamic_properties_layout, buttons_layout;
  
  // Functions described in `add-script.cpp`:
  ScriptMetadata *get_script_from_metadata();
};

#endif
