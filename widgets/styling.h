#ifndef STYLING_H
#define STYLING_H

#include <QApplication>
#include <QColor>
#include <QFontMetrics>
#include <QPalette>
#include <QString>

/*!
 * @class Styling
 * @brief Looks at what colors are set by the system theme, and allows widgets
 * to build their appearance on this basis.
 */
class Styling {
public:
  // Objects:
  QColor bg_color;
  QColor fg_color;
  QColor hl_color;
  QString css_bg_color;
  QString css_fg_color;
  QString css_hl_color;
  bool light_theme;
  QFontMetrics *metrics = nullptr;

  // Constants:
  inline static const QString css_blank = "rgb(%1,%2,%3)";

  inline static const QString css_light_sb = "#1b1b1b"; /*!< Light themed scroll bar color. */
  inline static const QString css_dark_sb  = "#e4e4e4";

  inline static const QString css_light_wbg = "#e7e8eb"; /*!< Light themed window bg. */
  inline static const QString css_dark_wbg  = "#282c34";

  inline static const QString css_light_sm = "#d1d2d4";   /*!< Light themed selected action. */
  inline static const QString css_light_pm = "#aeafb1"; /*!< ...pressed action. */
  inline static const QString css_dark_sm  = "#595c66";
  inline static const QString css_dark_pm  = "#6e717e";

  inline static const QString css_scroll_bar =
    "QScrollBar:vertical { background: transparent; border: none; width: 5px; margin: 0px 0px "
    "5px 0px; } QScrollBar::handle:vertical { border: 2px solid %1; background: %1; width: 5px;"
    "border-radius: 1px; } QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { width: 0px;"
    "height: 0px; }";

  // Functions:
  /*! @brief Determines theme colors using the current palette. */
  void calculate_colors() {
    metrics = new QFontMetrics(QApplication::font());
    const auto palette = QApplication::palette();
    bg_color = palette.color(QPalette::Active, QPalette::Window);
    fg_color = palette.color(QPalette::Active, QPalette::WindowText);
    hl_color = palette.color(QPalette::Active, QPalette::Highlight);
    css_bg_color = css_blank.arg(bg_color.red())
                            .arg(bg_color.green())
                            .arg(bg_color.blue());
    css_fg_color = css_blank.arg(fg_color.red())
                            .arg(fg_color.green())
                            .arg(fg_color.blue());
    css_hl_color = css_blank.arg(hl_color.red())
                            .arg(hl_color.green())
                            .arg(hl_color.blue());
    if (bg_color.lightness() < 127) light_theme = false;
    else light_theme = true;
  }
};

extern Styling styling;

#endif
