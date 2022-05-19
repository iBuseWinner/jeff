#include "phrase-editor.h"

/*! 
 * @brief The constructor. 
 * @details Overview layout scheme:
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
 * @details Selector layout scheme:
 * <--------------->
 * [  Phrase list  ]
 * [    <  Back    ]
 * <--------------->
 */
PhraseEditor::PhraseEditor(Basis *_basis, QWidget *parent, ModalHandler *m_handler)
    : ScrollFreezerWidget(parent), basis(_basis), _m_handler(m_handler) {
  _m_handler->setPrisoner(this);
  // Sets up overview widget.
  overview = new PhraseEditorOverview(basis);
  connect(overview, &PhraseEditorOverview::open_brief, this, [this](QTreeWidgetItem *item) {
    open_brief(item);
  });
  connect(&(overview->new_phrase), &Button::clicked, this, &PhraseEditor::add_new_phrase);
  connect(&(overview->phrases_list), &List::itemDoubleClicked, this, &PhraseEditor::open_brief);
  connect(&(overview->close_editor), &Button::clicked, this, [this] { _m_handler->closePrisoner(); });
  // Sets up brief widget.
  brief = new PhraseEditorBrief(basis);
  connect(&(brief->back_to_overview), &Button::clicked, this, &PhraseEditor::close_brief);
  connect(brief, &PhraseEditorBrief::open_brief, this, [this](QTreeWidgetItem *item) {
    open_brief(item);
  });
  connect(brief, &PhraseEditorBrief::add_phrase_and_send_id, this, &PhraseEditor::add_phrase_and_send_id);
  connect(brief, &PhraseEditorBrief::open_selector, this, &PhraseEditor::open_selector);
  connect(brief, &PhraseEditorBrief::update_phrases, overview, &PhraseEditorOverview::update_phrases);
  brief->hide();
  // Sets up selector widget.
  selector = new PhraseEditorSelector(basis);
  connect(&(selector->back_to_brief), &Button::clicked, this, &PhraseEditor::close_selector);
  connect(selector, &PhraseEditorSelector::selected, this, [this](int address) {
    brief->waits_for_choosed(address);
    close_selector();
  });
  selector->hide();
  // Shows overview.
  editor_layout.setSpacing(0);
  editor_layout.setMargin(0);
  editor_layout.addWidget(overview);
  setLayout(&editor_layout);
  overview->setFixedWidth(480);
  overview->fill_databases();
}

/*! @brief Opens information about the phrase. */
void PhraseEditor::open_brief(QTreeWidgetItem *item, int column) {
  Q_UNUSED(column)
  if (not mode_mutex.try_lock()) return;
  brief->setFixedWidth(overview->width() - 6);
  overview->hide();
  editor_layout.replaceWidget(overview, brief);
  brief->setup(overview->selected_source(), overview->phrases, item->text(0).toInt());
  brief->show();
  mode = BriefMode;
  mode_mutex.unlock();
}

/*! @brief Opens the phrase editor at its address in the source. */
void PhraseEditor::open_brief_by_address(int address) {
  if (not mode_mutex.try_lock()) return;
  brief->setFixedWidth(overview->width());
  overview->hide();
  editor_layout.replaceWidget(overview, brief);
  overview->update_phrases();
  brief->setup(overview->selected_source(), overview->phrases, address);
  brief->show();
  mode = BriefMode;
  mode_mutex.unlock();
}

/*! @brief Opens a list of phrases to choose from. */
void PhraseEditor::open_selector() {
  if (not mode_mutex.try_lock()) return;
  selector->setFixedWidth(overview->width());
  selector->setFixedHeight(brief->height());
  brief->hide();
  editor_layout.replaceWidget(brief, selector);
  selector->set_phrases(overview->phrases);
  selector->show();
  mode = SelectorMode;
  mode_mutex.unlock();
}

/*! @brief Closes the phrase list and returns to the phrase editor. */
void PhraseEditor::close_selector() {
  if (not mode_mutex.try_lock()) return;
  selector->hide();
  editor_layout.replaceWidget(selector, brief);
  brief->show();
  mode = BriefMode;
  mode_mutex.unlock();
}

/*! @brief Returns to the list of expressions. */
void PhraseEditor::close_brief() {
  if (not mode_mutex.try_lock()) return;
  brief->hide();
  editor_layout.replaceWidget(brief, overview);
  overview->update_phrases();
  overview->show();
  mode = OverviewMode;
  mode_mutex.unlock();
}

/*! @brief Adds a new phrase and opens the editor. */
void PhraseEditor::add_new_phrase() {
  auto source = overview->selected_source();
  if (source.path.isEmpty()) return;
  auto id = basis->sql->create_new_phrase(source, tr("New phrase"));
  if (id == -1) return;
  open_brief_by_address(id);
}

/*! @brief TBD */
void PhraseEditor::add_phrase_and_send_id() {
  auto source = overview->selected_source();
  if (source.path.isEmpty()) return;
  auto id = basis->sql->create_new_phrase(source, tr("New phrase"));
  if (id == -1) return;
  brief->waits_for_choosed(id);
  open_brief_by_address(id);
}
