#ifndef PHRASE_EDITOR_H
#define PHRASE_EDITOR_H

#include "core-kit/basis.hpp"
#include "core-kit/model/phrase.hpp"
#include "core-kit/model/source.hpp"
#include "dialogues/dialog.hpp"
#include "dialogues/modal-handler.hpp"
#include "dialogues/phrase-editor/brief.hpp"
#include "dialogues/phrase-editor/overview.hpp"
#include "dialogues/phrase-editor/selector.hpp"
#include "widgets/scrollfreezerwidget.hpp"
#include "widgets/styling.hpp"
#include "widgets/layouts/grid.hpp"
#include <QGridLayout>
#include <QMutex>
#include <QWidget>

/*! @enum PhraseEditorCurrentMode
 *  @brief Tells whether the widget is currently showing either a list of expressions,
 *  or an expression editor.  */
enum PhraseEditorCurrentMode { PEOverviewMode, PEBriefMode, PESelectorMode };

/*! @class PhraseEditor
 *  @brief Allows you to view, add, delete and edit the expressions that Jeff operates on.  */
class PhraseEditor : public Dialog {
  Q_OBJECT
  Q_DISABLE_COPY(PhraseEditor)
public:
  // Functions described in `phrase-editor.cpp`:
  PhraseEditor(Basis *_basis, QWidget *parent = nullptr, ModalHandler *mhandler = nullptr);

private:
  // Objects:
  Basis *basis = nullptr;
  PhraseEditorCurrentMode mode = PEOverviewMode;
  GridLt *editor_layout = nullptr;
  PhraseEditorOverview *overview = nullptr;
  PhraseEditorBrief *brief = nullptr;
  PhraseEditorSelector *selector = nullptr;
  
  // Functions described in `phrase-editor.cpp`:
  void open_brief(QTreeWidgetItem *item, int column = 0);
  void open_brief_by_address(int address);
  void open_selector();
  void close_selector();
  void close_brief();
  void add_new_phrase();
  void add_phrase_and_send_id();
};

#endif
