#ifndef SCRIPT_DIALOG_H
#define SCRIPT_DIALOG_H

#include "core-kit/basis.hpp"
#include "core-kit/extensions-manager.hpp"
#include "dialogues/dialog.hpp"
#include "dialogues/modal-handler.hpp"
#include "dialogues/phrase-editor/scripteditor.hpp"
#include "widgets/button.hpp"
#include "widgets/combobox.hpp"
#include "widgets/editlist.hpp"
#include "widgets/lineedit.hpp"
#include "widgets/list.hpp"
#include "widgets/scrollfreezerwidget.hpp"
#include "widgets/layouts/grid.hpp"
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
  // Objects:
  ReactScriptEditor *script_editor = nullptr;
  
  // Functions described in `add-script.cpp`:
  AddScriptDialog(QWidget *parent = nullptr, ModalHandler *mhandler = nullptr, Basis *_basis = nullptr);
};

#endif
