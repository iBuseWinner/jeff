#include "brief.h"

/*! @brief The constructor. */
PhraseEditorBrief::PhraseEditorBrief(Basis *_basis, QWidget *parent) : QScrollArea(parent), basis(_basis) {
  auto header_font = QApplication::font();
  header_font.setPointSize(14);
  header_font.setBold(true);
  header.setFont(header_font);
  header.setWordWrap(true);
  edit_expression.setText(tr("Edit text"));
  edit_expression.setIcon(
    QIcon::fromTheme("edit", QIcon(":/arts/icons/16/document-edit.svg")));
  connect(&edit_expression, &Button::clicked, this, &PhraseEditorBrief::edit_phrase_text);
  phrase_expression_edit_save.setIcon(
    QIcon::fromTheme("dialog-ok-apply", QIcon(":/arts/icons/16/dialog-ok-apply.svg")));
  connect(&phrase_expression_edit_save, &Button::clicked, this, &PhraseEditorBrief::save_phrase_text);
  back_to_selector.setText(tr("Back"));
  back_to_selector.setIcon(
    QIcon::fromTheme("go-previous", QIcon(":/arts/icons/16/go-previous.svg")));
  widget_layout.setSpacing(0);
  widget_layout.setMargin(0);
  widget_layout.addWidget(&header);
  widget_layout.addWidget(&edit_expression);
  widget_layout.addItem(new QSpacerItem(0, 0, QSizePolicy::Fixed, QSizePolicy::Expanding));
  widget_layout.addWidget(&back_to_selector);
  area_widget = new QWidget();
  area_widget->setLayout(&widget_layout);
  phrase_expression_edit_layout.addWidget(&phrase_expression_edit_line);
  phrase_expression_edit_layout.addWidget(&phrase_expression_edit_save);
  phrase_expression_edit_widget.setLayout(&phrase_expression_edit_layout);
  phrase_expression_edit_widget.hide();
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setFocusPolicy(Qt::NoFocus);
  setFrameStyle(QFrame::NoFrame);
  setFrameShadow(QFrame::Plain);
  setWidgetResizable(true);
  setWidget(area_widget);
}

/*! @brief Sets current phrase. */
void PhraseEditorBrief::set_current_phrase(Phrase _phrase) {
  phrase = _phrase;
  header.setText(phrase.expression);
}

/*! @brief Sets current source. */
void PhraseEditorBrief::set_current_source(Source _source) { source = _source; }

/*! @brief Edits the text of a phrase. */
void PhraseEditorBrief::edit_phrase_text() {
  edit_expression.setEnabled(false);
  back_to_selector.setEnabled(false);
  edit_expression.hide();
  header.hide();
  phrase_expression_edit_line.setText(header.text());
  widget_layout.replaceWidget(&header, &phrase_expression_edit_widget);
  phrase_expression_edit_widget.show();
}

/*! @brief Saves new text of a phrase. */
void PhraseEditorBrief::save_phrase_text() {
  auto text = phrase_expression_edit_line.text();
  if (text.isEmpty()) return;
  phrase.expression = text;
  if (not basis->sql->update_expression(source, phrase.expression, phrase.address)) return;
  phrase_expression_edit_widget.hide();
  header.setText(phrase.expression);
  widget_layout.replaceWidget(&phrase_expression_edit_widget, &header);
  header.show();
  edit_expression.show();
  edit_expression.setEnabled(true);
  back_to_selector.setEnabled(true);
}
