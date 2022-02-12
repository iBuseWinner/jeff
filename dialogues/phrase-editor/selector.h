#ifndef PHRASE_EDITOR_SELECTOR_H
#define PHRASE_EDITOR_SELECTOR_H

#include "core/basis.h"
#include "core/model/phrase.h"
#include "core/model/source.h"
#include "widgets/button.h"
#include "widgets/combobox.h"
#include "widgets/explanationlabel.h"
#include "widgets/list.h"
#include "widgets/menu.h"
#include <QAction>
#include <QCursor>
#include <QGridLayout>
#include <QPoint>
#include <QTreeWidgetItem>
#include <QWidget>

class PhraseEditorSelector : public QWidget {
  Q_OBJECT
  Q_DISABLE_COPY(PhraseEditorSelector)
public:
  // Objects:
  Button new_phrase, close_editor;
  List phrases_list;
  
  // Functions described in `selector.cpp`:
  PhraseEditorSelector(Basis *_basis, QWidget *parent = nullptr);
  void fill_databases();
  Source selected_source();

signals:
  QTreeWidgetItem *open_brief(QTreeWidgetItem *item);
  
private:
  // Objects:
  Basis *basis = nullptr;
  QGridLayout selector_layout; /*!< Top-level widgets. */
  ComboBox databases, tables;
  ExplanationLabel double_click_explain;
  Menu phrase_context_menu; /*!< Context menu. */
  QAction edit_phrase_action, remove_phrase_action;
  
  // Functions described in `selector.cpp`:
  void show_context_menu(const QPoint &pos);
  void fill_tables(const Sources &sources);
  void fill_phrases(const Sources &sources);
};

#endif
