#ifndef LINEEDIT_H
#define LINEEDIT_H

#include "widgets/menu.hpp"
#include "widgets/styling.hpp"
#include <QAction>
#include <QLineEdit>
#include <QWidget>

/*! @class LineEdit
 *  @brief The text entry line.  */
class LineEdit : public QLineEdit {
  Q_OBJECT
  Q_DISABLE_COPY(LineEdit)
public:
  // Functions described in `lineedit.cpp`:
  LineEdit(QWidget *parent = nullptr);
  void hide_border();

private:
  // Constants:
  inline static const QString style_sheet = "LineEdit { background-color: %1; color: %2; border-width: 1px; border-style: solid; border-color: %3; border-radius: 6px; } LineEdit:disabled { border-color: %1; }";
  inline static const QString wob_style_sheet = "background-color: %1; color: %2;";
  
  // Objects:
  Menu *context_menu = nullptr;
  QAction *delete_text_action = nullptr,
          *cut_text_action = nullptr,
          *copy_text_action = nullptr,
          *paste_text_action = nullptr,
          *select_all_text_action = nullptr;
  
  // Functions described in `lineedit.cpp`:
  void setup_context_menu();
};

#endif
