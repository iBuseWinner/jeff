#ifndef SCRIPT_DIALOG_H
#define SCRIPT_DIALOG_H

#include "core/basis.h"
#include "dialogues/modal-handler.h"
#include "widgets/button.h"
#include "widgets/combobox.h"
#include "widgets/lineedit.h"
#include "widgets/list.h"
#include <QCheckBox>
#include <QGridLayout>
#include <QLabel>
#include <QWidget>

/*! @class ScriptDialog
 *  @brief TBD  */
class ScriptDialog : public QWidget {
  Q_OBJECT
  Q_DISABLE_COPY(ScriptDialog)
public:
  // Functions described in `scripts.cpp`:
  ScriptDialog(Basis *_basis, QWidget *parent, ModalHandler *m_handler);
  void change_stype();
  
private:
  Basis *basis = nullptr;
  ModalHandler *_m_handler = nullptr;
  QLabel path_info, stype_info;
  Button path_input;
  ComboBox stype_input;
  QLabel *send_adprops_info = nullptr, *server_addr_info = nullptr,
    *server_port_info = nullptr, *memory_cells_info = nullptr;
  QCheckBox *send_adprops_input = nullptr;
  LineEdit *server_addr_input = nullptr, *server_port_input = nullptr;
  List *memory_cells_list = nullptr;
  Button *add_memory_cell_btn = nullptr, *remove_memory_cell_btn = nullptr;
  Button cancel_btn, save_btn;
  QWidget dynamic_properties_widget;
  QGridLayout main_layout, dynamic_properties_layout;
};

#endif
