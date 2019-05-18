#ifndef A_LINEEDIT_H
#define A_LINEEDIT_H

#include <QLineEdit>
#include <QWidget>

/*!
 * Class: ALineEdit
 * The text entry line.
 */
class ALineEdit : public QLineEdit {
 public:
  // Functions:
  ALineEdit(QWidget *parent = nullptr);

 private:
  Q_DISABLE_COPY(ALineEdit)
};

#endif  // A_LINEEDIT_H
