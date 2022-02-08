#include "list.h"

/*! @brief The constructor. */
List::List(QWidget *parent) : QTreeWidget(parent) {
  setAnimated(false);
  setFocusPolicy(Qt::NoFocus);
  setRootIsDecorated(false);
  setTextElideMode(Qt::ElideLeft);
  setStyleSheet(
    QString(
      "List, QHeaderView { background-color: %1; color: %2; } "
    ).arg(styling.css_bg_color).arg(styling.css_fg_color) +
    styling.css_scroll_bar.arg(styling.light_theme ? styling.css_light_sb : styling.css_dark_sb)
  );
  normal_portion = max_items_amount / 4;
  connect(verticalScrollBar(), &QScrollBar::rangeChanged, this, &List::scroll);
  connect(verticalScrollBar(), &QScrollBar::valueChanged, this, &List::show_items);
}

/*! @brief Adds a top-level element to the list, while ensuring that no more than a certain number 
 *  of elements are displayed on the screen.  */
void List::addTopLevelItem(QTreeWidgetItem *item) {
  items_counter++;
  top_level_items.append(item);
  QTreeWidget::addTopLevelItem(item);
  while (topLevelItemCount() > max_items_amount) takeTopLevelItem(0);
}

/*! @brief Clears the list of all elements. */
void List::clear() {
  while (takeTopLevelItem(0)) {}
  items_counter = 0;
  top_level_items.clear();
}

/*! @brief Makes it so that when you add as many elements as you like, the most recent ones
 *  are displayed on the screen.  */
void List::scroll(int min, int max) {
  verticalScrollBar()->setValue(max);
}

/*! @brief Shows neccessary widgets when scrolling. */
void List::show_items(int value) {
  if (not scroll_update.try_lock()) return;
  int total = topLevelItemCount();
  if (items_counter > total) {
    if (value == verticalScrollBar()->minimum()) {
      int min_index = top_level_items.indexOf(topLevelItem(0));
      short portion = min_index < normal_portion ? min_index : normal_portion;
      if (portion < 1) {
        scroll_update.unlock();
        return;
      }
      while (portion--) {
        takeTopLevelItem(total - 1);
        insertTopLevelItem(0, top_level_items[--min_index]);
      }
      scrollTo(model()->index(3, 0), QAbstractItemView::PositionAtTop);
    } else if (value == verticalScrollBar()->maximum()) {
      int max_index = top_level_items.indexOf(topLevelItem(topLevelItemCount() - 1));
      short portion = (top_level_items.length() - max_index - 1) < normal_portion ? 
        (top_level_items.length() - max_index - 1) : normal_portion;
      if (portion < 1) {
        scroll_update.unlock();
        return;
      }
      while (portion--) {
        takeTopLevelItem(0);
        insertTopLevelItem(total - 1, top_level_items[++max_index]);
      }
      scrollTo(model()->index(max_items_amount - 3, 0), QAbstractItemView::PositionAtBottom);
    }
  }
  scroll_update.unlock();
}
