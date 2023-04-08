#ifndef PHRASE_EDITOR_OVERVIEW_H
#define PHRASE_EDITOR_OVERVIEW_H

#include "core-kit/basis.hpp"
#include "core-kit/model/phrase.hpp"
#include "core-kit/model/source.hpp"
#include "widgets/button.hpp"
#include "widgets/combobox.hpp"
#include "widgets/explanationlabel.hpp"
#include "widgets/list.hpp"
#include "widgets/menu.hpp"
#include <QAction>
#include <QCursor>
#include <QGridLayout>
#include <QMutableListIterator>
#include <QPoint>
#include <QTreeWidgetItem>
#include <QWidget>

/*! @class PhraseEditorOverview
 *  @brief List of phrases in Jeff's database with the ability to edit.  */
class PhraseEditorOverview : public QWidget {
  Q_OBJECT
  Q_DISABLE_COPY(PhraseEditorOverview)
public:
  // Objects:
  Button new_phrase, close_editor;
  List phrases_list;
  Phrases phrases;
  
  // Functions described in `overview.cpp`:
  PhraseEditorOverview(Basis *_basis, QWidget *parent = nullptr);
  void fill_databases();
  Source selected_source();
  void update_phrases();

signals:
  /*! @brief Asks to open detailed information about the phrase. */
  QTreeWidgetItem *open_brief(QTreeWidgetItem *item);
  
private:
  // Objects:
  Basis *basis = nullptr;
  QGridLayout overview_layout; /*!< Top-level widgets. */
  ComboBox databases, tables;
  ExplanationLabel double_click_explain;
  Menu phrase_context_menu; /*!< Context menu. */
  QAction edit_phrase_action, remove_phrase_action;
  
  // Functions described in `overview.cpp`:
  void show_context_menu(const QPoint &pos);
  void fill_tables(const Sources &sources);
  void fill_phrases();
};

#endif
