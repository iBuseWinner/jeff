#include "selector.hpp"

/*! @brief The constructor. */
PhraseEditorSelector::PhraseEditorSelector(Basis *_basis, QWidget *parent)
    : QWidget(parent), basis(_basis) {
  auto header_font = QApplication::font();
  header_font.setPointSize(14);
  header_font.setBold(true);
  header.setFont(header_font);
  header.setWordWrap(true);
  header.setText(tr("Select a phrase..."));
  phrases_list.setHeaderLabels({tr("Address"), tr("Phrase")});
  phrases_list.setWordWrap(true);
  phrases_list.setContextMenuPolicy(Qt::CustomContextMenu);
  connect(&phrases_list, &List::itemDoubleClicked, this, [this](QTreeWidgetItem *item) {
    emit selected(item->text(0).toInt());
  });
  back_to_brief.setText(tr("Back"));
  back_to_brief.setIcon(QIcon::fromTheme("go-previous", QIcon(":/arts/icons/16/go-previous.svg")));
  connect(&back_to_brief, &Button::clicked, this, [this] { emit back(); });
  setLayout(VLineLt::another()->spacing()->addw(&header)->addw(&phrases_list)->addw(&back_to_brief));
}

/*! @brief Puts phrases from the source into the phrase list. */
void PhraseEditorSelector::set_phrases(Phrases _phrases) {
  phrases = _phrases;
  phrases_list.clear();
  QTreeWidgetItem *parent = phrases_list.invisibleRootItem();
  for (auto phrase : phrases) {
    phrases_list.addTopLevelItem(
      new QTreeWidgetItem(parent, {QString::number(phrase.address), phrase.expression})
    );
  }
}
