#ifndef A_LINEEDIT_H
#define A_LINEEDIT_H

#include <QLineEdit>
#include <QWidget>

class ALineEdit : public QLineEdit {
 public:
  ALineEdit(QWidget *p = nullptr);

 private:
  Q_DISABLE_COPY(ALineEdit)
};

#endif  // A_LINEEDIT_H
