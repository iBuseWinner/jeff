#include "phrase-editor.h"

/*! 
 * @brief The constructor. 
 * @details Selector layout scheme:
 * <--------------->
 * {   Select db   }
 * { Select  table }
 * [  Phrase list  ]
 * <      Tip      >
 * [ New ] [ Close ]
 * <--------------->
 * @details Brief layout scheme:
 * <--------------->
 * {  Phrase text  }
 * [ Edit this one ]
 * [ Pre- ][ Post- ]
 * [Add...][ Add...]
 * [    <  Back    ]
 * <--------------->
 */
PhraseEditor::PhraseEditor(Basis *_basis, QWidget *parent, ModalHandler *m_handler)
    : ScrollFreezerWidget(parent), basis(_basis), _m_handler(m_handler) {
  _m_handler->setPrisoner(this);
  // Sets up selector widget.
  selector = new PhraseEditorSelector(basis);
  connect(selector, &PhraseEditorSelector::open_brief, this, [this](QTreeWidgetItem *item) {
    open_brief(item);
  });
  connect(&(selector->new_phrase), &Button::clicked, this, &PhraseEditor::add_new_phrase);
  connect(&(selector->phrases_list), &List::itemDoubleClicked, this, &PhraseEditor::open_brief);
  connect(&(selector->close_editor), &Button::clicked, this, [this] { _m_handler->closePrisoner(); });
  // Sets up brief widget.
  brief = new PhraseEditorBrief(basis);
  connect(&(brief->back_to_selector), &Button::clicked, this, &PhraseEditor::close_brief);
  brief->hide();
  // Shows selector.
  editor_layout.setSpacing(0);
  editor_layout.setMargin(0);
  editor_layout.addWidget(selector);
  setLayout(&editor_layout);
  selector->setFixedWidth(480);
  selector->fill_databases();
}

/*! @brief Opens information about the phrase. */
void PhraseEditor::open_brief(QTreeWidgetItem *item, int column) {
  Q_UNUSED(column)
  if (not mode_mutex.try_lock()) return;
  brief->setFixedWidth(selector->width());
  brief->setFixedHeight(selector->height());
  selector->hide();
  editor_layout.replaceWidget(selector, brief);
  brief->set_current_phrase(
    basis->sql->get_phrase_by_address(selector->selected_source(), item->text(0).toInt())
  );
  brief->set_current_source(selector->selected_source());
  brief->set_phrases(selector->phrases);
  brief->show();
  mode = BriefMode;
  mode_mutex.unlock();
}

/*! @brief Returns to the list of expressions. */
void PhraseEditor::close_brief() {
  if (not mode_mutex.try_lock()) return;
  brief->hide();
  editor_layout.replaceWidget(brief, selector);
  selector->update_phrases();
  selector->show();
  mode = SelectorMode;
  mode_mutex.unlock();
}

/*! @brief Adds a new phrase and opens the editor. */
void PhraseEditor::add_new_phrase() {
  auto source = selector->selected_source();
  if (source.path.isEmpty()) return;
  auto id = basis->sql->create_new_phrase(source, tr("New phrase"));
  if (id == -1) return;
  auto *phrase_item = new QTreeWidgetItem(
    selector->phrases_list.invisibleRootItem(), {QString::number(id), tr("New phrase")});
  selector->phrases_list.addTopLevelItem(phrase_item);
  open_brief(phrase_item, 0);
}
