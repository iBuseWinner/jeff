#include "list.h"

/*!
 * @fn List::List
 * @brief The constructor.
 * @param[in,out] parent QObject parent
 */
List::List(QWidget *parent) : QTreeWidget(parent) {
  setAnimated(false);
  setStyleSheet(
    QString(
      "List, QHeaderView { background-color: %1; color: %2; } "
    ).arg(styling.css_bg_color).arg(styling.css_fg_color) +
    styling.css_scroll_bar.arg(styling.light_theme ? styling.css_light_sb : styling.css_dark_sb)
  );
}
