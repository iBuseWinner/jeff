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
#include "widgets/scripteditor.h"
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
  
private:
  // Objects:
  ModalHandler *_m_handler = nullptr;
};

#endif
