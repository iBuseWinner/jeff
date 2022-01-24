#ifndef LINEEDIT_H
#define LINEEDIT_H

#include <QLineEdit>
#include <QWidget>

/*!
 * @class LineEdit
 * @brief The text entry line.
 */
class LineEdit : public QLineEdit {
  Q_OBJECT
  Q_DISABLE_COPY(LineEdit)
public:
  // Functions described in 'lineedit.cpp':
  LineEdit(QWidget *parent = nullptr);

private:
  // Constants:
  inline static const QString style_sheet = "background-color: rgb(255, 255, 255); color: rgb(0, 0, 0);";
};

#endif
