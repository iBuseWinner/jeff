#ifndef PHRASE_EDITOR_SELECTOR_H
#define PHRASE_EDITOR_SELECTOR_H

#include "core-kit/basis.hpp"
#include "core-kit/model/phrase.hpp"
#include "core-kit/model/source.hpp"
#include "widgets/button.hpp"
#include "widgets/combobox.hpp"
#include "widgets/explanationlabel.hpp"
#include "widgets/list.hpp"
#include "widgets/menu.hpp"
#include "widgets/layouts/linears.hpp"
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
