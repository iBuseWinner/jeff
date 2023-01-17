#ifndef SCRIPT_DIALOG_H
#define SCRIPT_DIALOG_H

#include "core-kit/basis.h"
#include "core-kit/extensions-manager.h"
#include "dialogues/dialog.h"
#include "dialogues/modal-handler.h"
#include "widgets/button.h"
#include "widgets/combobox.h"
#include "widgets/editlist.h"
#include "widgets/lineedit.h"
#include "widgets/list.h"
#include "widgets/scripteditor.h"
#include "widgets/scrollfreezerwidget.h"
#include "widgets/layouts/grid.h"
#include <QCheckBox>
#include <QFileDialog>
#include <QLabel>
#include <QLayoutItem>
#include <QSpinBox>

/*! @class AddScriptDialog
 *  @brief Adds information about scripts to Jeff.  */
class AddScriptDialog : public Dialog {
  Q_OBJECT
  Q_DISABLE_COPY(AddScriptDialog)
public:  
  // Functions described in `add-script.cpp`:
  AddScriptDialog(QWidget *parent = nullptr, ModalHandler *mhandler = nullptr,
                  Basis *_basis = nullptr, ExtensionsManager *_em = nullptr);
};

#endif
