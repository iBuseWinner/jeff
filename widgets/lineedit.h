#ifndef LINEEDIT_H
#define LINEEDIT_H

#include "widgets/styling.h"
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
  // Functions described in `lineedit.cpp`:
  LineEdit(QWidget *parent = nullptr);

private:
  // Constants:
  inline static const QString style_sheet = "background-color: %1; color: %2; border-width: 1px; border-style: solid; border-color: %3; border-radius: 6px;";
};

#endif
