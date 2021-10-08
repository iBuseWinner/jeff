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
public:
  // Functions described in 'lineedit.cpp':
  explicit LineEdit(QWidget *parent = nullptr);

private:
  Q_DISABLE_COPY(LineEdit)

  // Constants:
  inline static const QString style_sheet =
      "background-color: rgb(255, 255, 255); color: rgb(0, 0, 0);";
};

#endif
