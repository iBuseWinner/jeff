#ifndef PHRASE_EDITOR_H
#define PHRASE_EDITOR_H

#include "core/basis.h"
#include "core/model/phrase.h"
#include "core/model/source.h"
#include "dialogues/phrase-editor/brief.h"
#include "dialogues/phrase-editor/selector.h"
#include "dialogues/modal-handler.h"
#include "widgets/scrollfreezerwidget.h"
#include "widgets/styling.h"
#include <QGridLayout>
#include <QMutex>
#include <QWidget>

/*!
 * @enum PhraseEditorCurrentMode
 * @brief Tells whether the widget is currently showing either a list of expressions, or an expression editor.
 */
enum PhraseEditorCurrentMode { SelectorMode, BriefMode };

/*!
 * @class PhraseEditor
 * @brief Allows you to view, add, delete and edit the expressions that Jeff operates on.
 */
class PhraseEditor : public ScrollFreezerWidget {
  Q_OBJECT
  Q_DISABLE_COPY(PhraseEditor)
public:
  // Functions described in `phrase-editor.cpp`:
  PhraseEditor(Basis *_basis, QWidget *parent = nullptr, ModalHandler *m_handler = nullptr);

private:
  // Objects:
  ModalHandler *_m_handler = nullptr;
  Basis *basis = nullptr;
  QMutex mode_mutex;
  PhraseEditorCurrentMode mode = SelectorMode;
  QGridLayout editor_layout;
  PhraseEditorSelector *selector = nullptr;
  PhraseEditorBrief *brief = nullptr;
  
  // Functions described in `phrase-editor.cpp`:
  void open_brief(QTreeWidgetItem *item, int column = 0);
  void close_brief();
  void add_new_phrase();
};

#endif
