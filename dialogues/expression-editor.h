#ifndef EXPRESSION_EDITOR_H
#define EXPRESSION_EDITOR_H

#include "core/basis.h"
#include "core/model/expression.h"
#include "core/model/source.h"
#include "dialogues/modal-handler.h"
#include "widgets/button.h"
#include "widgets/combobox.h"
#include "widgets/explanationlabel.h"
#include "widgets/lineedit.h"
#include "widgets/list.h"
#include "widgets/menu.h"
#include "widgets/scrollfreezerwidget.h"
#include "widgets/styling.h"
#include <QAction>
#include <QApplication>
#include <QCursor>
#include <QFont>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMutex>
#include <QPoint>
#include <QScrollArea>
#include <QStringList>
#include <QVariant>
#include <QVBoxLayout>
#include <QWidget>

/*!
 * @enum ExpressionEditorCurrentMode
 * @brief Tells whether the widget is currently showing either a list of expressions, or an expression editor.
 */
enum ExpressionEditorCurrentMode { SelectorMode, BriefMode };

/*!
 * @class ExpressionEditor
 * @brief Allows you to view, add, delete and edit the expressions that Jeff operates on.
 */
class ExpressionEditor : public ScrollFreezerWidget {
  Q_OBJECT
  Q_DISABLE_COPY(ExpressionEditor)
public:
  // Functions described in `expression-editor.cpp`:
  ExpressionEditor(Basis *_basis, QWidget *parent = nullptr, ModalHandler *m_handler = nullptr);

private:
  // Objects:
  Basis *basis = nullptr;
  ModalHandler *_m_handler = nullptr;
  QList<QPair<int, QString>> selector_data;
  QMutex mode_mutex;
  ExpressionEditorCurrentMode mode = SelectorMode;
  Sources current_sources;
  // Common widgets.
  QGridLayout editor_layout;
  // Selector widgets.
  QWidget selector;
  QGridLayout selector_layout;
  ComboBox databases, tables;
  List expressions;
  Menu expressions_context_menu;
  QAction edit_expression_action, remove_expression_action;
  ExplanationLabel double_click_explain;
  Button new_expression, close_editor;
  // Brief widgets.
  QScrollArea brief_area;
  QWidget brief_area_widget;
  QGridLayout brief_area_layout;
  QLabel brief_header;
  QHBoxLayout expression_edit_line_layout;
  LineEdit expression_edit;
  Button accept_expression_text;
  Button back_to_selector;
  
  // Functions described in `expression-editor.cpp`:
  void fill_databases();
  void fill_tables(const Sources &sources);
  Source selected_source();
  void fill_expressions(const Sources &sources);
  void open_brief(QTreeWidgetItem *item, int column);
  void close_brief();
  void show_selector_expr_context_menu(const QPoint &pos);
  void add_new_expression();
  void save_expression_text();
};

#endif
