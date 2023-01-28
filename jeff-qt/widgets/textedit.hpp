#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include "widgets/menu.hpp"
#include "widgets/styling.hpp"
#include <QAction>
#include <QKeyEvent>
#include <QScrollBar>
#include <QTextEdit>
#include <QWidget>

/*! @class TextEdit
 *  @brief The text entry multiline.  */
class TextEdit : public QTextEdit {
  Q_OBJECT
  Q_DISABLE_COPY(TextEdit)
public:
  // Functions described in `textedit.cpp`:
  TextEdit(QWidget *parent = nullptr);
  void hide_border();
  void backspace();
  void insert(const QString &text);
  QString text();
  void clear(int presize = -1);

protected:
  // Functions described in `textedit.cpp`:
  void keyPressEvent(QKeyEvent *event) override;

signals:
  void enter_triggered();

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
  int curr_height = -1;
  uint line_cntr = 1;
  
  // Functions described in `textedit.cpp`:
  void setup_context_menu();
  int text_height();
  void resize_on_text_height();
};

#endif
