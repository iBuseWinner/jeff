#ifndef PHRASE_EDITOR_BRIEF_H
#define PHRASE_EDITOR_BRIEF_H

#include "scripteditor.hpp"
#include "core-kit/basis.hpp"
#include "widgets/button.hpp"
#include "widgets/explanationlabel.hpp"
#include "widgets/lineedit.hpp"
#include "widgets/list.hpp"
#include "widgets/menu.hpp"
#include "widgets/styling.hpp"
#include "widgets/layouts/linears.hpp"
#include <QApplication>
#include <QCheckBox>
#include <QCursor>
#include <QLabel>
#include <QPoint>
#include <QScrollArea>
#include <QWidget>

/*! @enum PhraseEditorBriefWaitsFor
 *  @brief Indicates what the phrase editor expects from the selector widget.  */
enum PhraseEditorBriefWaitsFor { Nothing, Activator, Reagent };

/*! @class PhraseEditorBrief
 *  @brief Displays all phrase properties.  */
class PhraseEditorBrief : public QScrollArea {
  Q_OBJECT
  Q_DISABLE_COPY(PhraseEditorBrief)
public:
  // Objects:
  Button back_to_overview;
  
  // Functions described in `brief.cpp`:
  PhraseEditorBrief(Basis *_basis, QWidget *parent = nullptr);
  bool setup(Source _source, Phrases _phrases, int address);
  void waits_for_choosed(int address);
  
signals:
  /*! @brief Opens another phrase. */
  QTreeWidgetItem *open_brief(QTreeWidgetItem *item);
  /*! @brief Adds new phrase and appends it for activator's/reagent's list. */
  void add_phrase_and_send_id();
  /*! @brief Opens selector to connect another phrase to this one. */
  void open_selector();
  /*! @brief Forces the list of expressions held in memory to be updated. */
  void update_phrases();
  
private:
  // Objects:
  Basis *basis = nullptr;
  Phrase phrase;
  Source source;
  Phrases phrases;
  QWidget *area_widget = nullptr; /*!< Top-level widgets. */
  VLineLt *widget_layout = nullptr;
  QLabel header;
  Button edit_expression;
  ExplanationLabel address_label;
  QCheckBox exec_checkbox;
  List activators_list, reagents_list;
  QWidget phrase_expression_edit_widget; /*! Expression editor widgets. */
  LineEdit phrase_expression_edit_line;
  Button phrase_expression_edit_save;
  ScriptEditor *script_editor = nullptr;
  Menu phrases_context_menu; /*! Context menus. */
  QAction
    add_phrase_action, connect_phrase_action, edit_phrase_action, 
    disconnect_phrase_action, remove_phrase_action;
  PhraseEditorBriefWaitsFor waits_for = Nothing;
  
  // Functions described in `brief.cpp`:
  void edit_phrase_text();
  void save_phrase_text();
  void save_script(QString script_json);
  void change_exec(int state);
  void context_menu_for_activators(const QPoint &pos);
  void context_menu_for_reagents(const QPoint &pos);
  void resizeEvent(QResizeEvent *event) override;
};

#endif
