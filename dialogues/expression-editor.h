#ifndef EXPRESSION_EDITOR_H
#define EXPRESSION_EDITOR_H

#include "core/basis.h"
#include "core/model/expression.h"
#include "core/model/source.h"
#include "dialogues/modal-handler.h"
#include "widgets/button.h"
#include "widgets/combobox.h"
#include "widgets/list.h"
#include "widgets/styling.h"
#include <QApplication>
#include <QGridLayout>
#include <QScrollArea>
#include <QStringList>
#include <QVariant>
#include <QWidget>

/*!
 * @class ExpressionEditor
 * @brief Allows you to view, add, delete and edit the expressions that Jeff operates on.
 */
class ExpressionEditor : public QWidget {
  Q_OBJECT
  Q_DISABLE_COPY(ExpressionEditor)
public:
  // Functions described in `expression-editor.cpp`:
  ExpressionEditor(Basis *_basis, QWidget *parent = nullptr, ModalHandler *m_handler = nullptr);

private:
  // Objects:
  QScrollArea area;
  QWidget selector;
  QGridLayout selector_layout;
  QWidget *brief = nullptr;
  Basis *basis = nullptr;
  ModalHandler *_m_handler = nullptr;
  Button new_expression, close_editor;
  ComboBox databases, tables;
  List expressions;
  QList<QPair<int, QString>> selector_data;
  
  // Functions described in `expression-editor.cpp`:
//   void open_brief();
//   void close_brief();
  void fill_databases();
  void fill_tables(const Sources &sources);
  void fill_expressions(const Sources &sources);
};

#endif
