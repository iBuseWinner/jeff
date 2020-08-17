#ifndef A_LINEEDIT_H
#define A_LINEEDIT_H

#include <QLineEdit>
#include <QWidget>

/*!
 * @class ALineEdit
 * @brief The text entry line.
 */
class ALineEdit : public QLineEdit {
  Q_OBJECT
public:
  // Functions described in 'a_lineedit.cpp':
  explicit ALineEdit(QWidget *parent = nullptr);

private:
  Q_DISABLE_COPY(ALineEdit)

  // Constants:
  inline static const QString style_sheet = "color: rgb(0, 0, 0);";
};

#endif // A_LINEEDIT_H
