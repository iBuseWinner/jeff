#ifndef PHRASE_EDITOR_BRIEF_H
#define PHRASE_EDITOR_BRIEF_H

#include "core/basis.h"
#include "widgets/button.h"
#include "widgets/explanationlabel.h"
#include "widgets/lineedit.h"
#include <QApplication>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QWidget>

class PhraseEditorBrief : public QScrollArea {
  Q_OBJECT
  Q_DISABLE_COPY(PhraseEditorBrief)
public:
  // Objects:
  Button back_to_selector;
  
  // Functions described in `brief.cpp`:
  PhraseEditorBrief(Basis *_basis, QWidget *parent = nullptr);
  void set_current_phrase(Phrase _phrase);
  void set_current_source(Source _source);
  void set_phrases(Phrases _phrases);
  
private:
  // Objects:
  Basis *basis = nullptr;
  Phrase phrase;
  Source source;
  Phrases phrases;
  QWidget *area_widget = nullptr; /*!< Top-level widgets. */
  QVBoxLayout widget_layout;
  QLabel header;
  Button edit_expression;
  ExplanationLabel address_label;
  QCheckBox exec_checkbox;
  QWidget phrase_expression_edit_widget; /*! Expression editor widgets. */
  QHBoxLayout phrase_expression_edit_layout;
  LineEdit phrase_expression_edit_line;
  Button phrase_expression_edit_save;
  
  // Functions described in `brief.cpp`:
  void edit_phrase_text();
  void save_phrase_text();
  void change_exec(int state);
};

#endif
