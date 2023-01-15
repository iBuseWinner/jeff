#ifndef PHRASE_EDITOR_SELECTOR_H
#define PHRASE_EDITOR_SELECTOR_H

#include "core-kit/basis.h"
#include "core-kit/model/phrase.h"
#include "core-kit/model/source.h"
#include "widgets/button.h"
#include "widgets/combobox.h"
#include "widgets/explanationlabel.h"
#include "widgets/list.h"
#include "widgets/menu.h"
#include "widgets/layouts/linears.h"
#include <QAction>
#include <QApplication>
#include <QCursor>
#include <QLabel>
#include <QPoint>
#include <QTreeWidgetItem>
#include <QWidget>

/*! @class PhraseEditorSelector
 *  @brief Allows you to attach other phrases to one phrase.  */
class PhraseEditorSelector : public QWidget {
  Q_OBJECT
  Q_DISABLE_COPY(PhraseEditorSelector)
public:
  // Objects:
  List phrases_list;
  Phrases phrases;
  Button back_to_brief;
  
  // Functions described in `selector.cpp`:
  PhraseEditorSelector(Basis *_basis, QWidget *parent = nullptr);
  void set_phrases(Phrases _phrases);

signals:
  int selected(int address);
  void back();
  
private:
  // Objects:
  Basis *basis = nullptr;
  VLineLt *selector_layout = nullptr;
  QLabel header;
};

#endif
