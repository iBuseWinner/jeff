#ifndef A_EXPRESSIONLIST_H
#define A_EXPRESSIONLIST_H

#include <QListWidget>

/*!
 * @class AExpressionList
 * @brief List of expressions.
 * @details The class provides a widget with which the user can view expressions
 * from sources in databases, edit them, add and delete, change links to each
 * other.
 * @sa SQLite, Source, NLPmodule
 */
class AExpressionList : public QListWidget {
  Q_OBJECT
public:
  // Functions described in 'a_expressionlist.cpp':
  explicit AExpressionList(QWidget *parent = nullptr);
};

#endif // A_EXPRESSIONLIST_H
