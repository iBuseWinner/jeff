#ifndef AEXPRESSIONLIST_H
#define AEXPRESSIONLIST_H

#include <QListWidget>

class AExpressionList : public QListWidget {
  Q_OBJECT
public:
  explicit AExpressionList(QWidget *parent = nullptr);
};

#endif // A_EXPRESSIONLIST_H
