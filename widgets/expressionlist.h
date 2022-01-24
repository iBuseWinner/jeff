#ifndef EXPRESSIONLIST_H
#define EXPRESSIONLIST_H

#include <QListWidget>

/*!
 * @class ExpressionList
 * @brief List of expressions.
 * @details The class provides a widget with which the user can view expressions
 * from sources in databases, edit them, add and delete, change links to each
 * other.
 * @sa SQLite, Source, NLPmodule
 */
class ExpressionList : public QListWidget {
  Q_OBJECT
public:
  // Functions described in 'expressionlist.cpp':
  ExpressionList(QWidget *parent = nullptr);
};

#endif
